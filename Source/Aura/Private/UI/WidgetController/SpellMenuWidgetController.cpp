// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

USpellMenuWidgetController::USpellMenuWidgetController()
{
	SelectedAbility = {Ability_None, Ability_Status_Locked};
	CurrentSpellPoints = 0;
	bIsWaitingForEquipSelection = false;
}

void USpellMenuWidgetController::BroadcastInitialValues()
{
	/*Super::BroadcastInitialValues();*/

	BroadcastAbilityInfo();

	if (!GetAuraPlayerState()) { return; }
	OnSpellPointChange.Broadcast(AuraPlayerState->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	/*Super::BindCallbacksToDependencies();*/
	if (GetAuraPlayerState())
	{
		AuraPlayerState->OnSpellPointChangeDelegate.AddLambda([this](const int32 StatValue)
		{
			OnSpellPointChange.Broadcast(StatValue);
			CurrentSpellPoints = StatValue;

			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			FString CurrentLevelDesc;
			FString NextLevelDesc;
			GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedAbility.AbilityTag, CurrentLevelDesc, NextLevelDesc);
			ShouldEnableButtons(SelectedAbility.StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			OnSpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquip, CurrentLevelDesc, NextLevelDesc);
		});
	}
	
	GetAuraAbilitySystemComponent()->OnAbilityStatusTagChangeDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel)
	{
		if (SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.StatusTag = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			FString CurrentLevelDesc;
			FString NextLevelDesc;
			GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, CurrentLevelDesc, NextLevelDesc);
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			OnSpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquip, CurrentLevelDesc, NextLevelDesc);
		}
		
		if (AbilityInformation)
		{
			FAuraAbilityInfo Info = AbilityInformation->GetAbilityInfoByTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetAuraAbilitySystemComponent()->OnAbilityEquippedDelegate.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FGameplayTag& AbilityType = AbilityInformation->GetAbilityInfoByTag(AbilityTag, true).AbilityTypeTag;
	OnEndEquipSelection.Broadcast(AbilityType);
	bIsWaitingForEquipSelection = false;
	
	const int32 SpellPoints = GetAuraPlayerState()->GetSpellPoints();
	
	FGameplayTag StatusTag;
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetAbilitySpecFromAbilityTag(AbilityTag);
	
	const bool bIsTagValid = AbilityTag.IsValid();
	const bool bIsTagNone = AbilityTag.MatchesTagExact(Ability_None);
	const bool bIsAbilitySpecValid = AbilitySpec != nullptr;

	if (bIsTagNone || !bIsTagValid || !bIsAbilitySpecValid)
	{
		StatusTag = Ability_Status_Locked;
	}
	else
	{
		StatusTag = AuraASC->GetAbilityStatusTagFromSpec(*AbilitySpec);
	}

	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = StatusTag;
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	FString CurrentLevelDesc;
	FString NextLevelDesc;
	GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, CurrentLevelDesc, NextLevelDesc);
	ShouldEnableButtons(StatusTag, SpellPoints, bEnableSpendPoints, bEnableEquip);
	OnSpellGlobeSelected.Broadcast(bEnableSpendPoints, bEnableEquip, CurrentLevelDesc, NextLevelDesc);
}

void USpellMenuWidgetController::OnSpendSpellPointButtonPressed()
{
	if (GetAuraAbilitySystemComponent())
	{
		AuraASC->ServerSpendSpellPoint(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::OnEquipButtonPressed()
{
	const FGameplayTag& AbilityType = AbilityInformation->GetAbilityInfoByTag(SelectedAbility.AbilityTag, true).AbilityTypeTag;

	OnBeginEquipSelection.Broadcast(AbilityType);
	bIsWaitingForEquipSelection = true;

	if (SelectedAbility.StatusTag.MatchesTagExact(Ability_Status_Equipped))
	{
		SelectedSlotInputTag = GetAuraAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::OnEquippedSpellsRowPressed(const FGameplayTag& SlotInputTag, const FGameplayTag& AbilityTypeTag)
{
	if (!bIsWaitingForEquipSelection) { return; }

	const FGameplayTag& SelectedAbilityType = AbilityInformation->GetAbilityInfoByTag(SelectedAbility.AbilityTag).AbilityTypeTag;
	if (!AbilityTypeTag.MatchesTagExact(SelectedAbilityType)) { return; }

	GetAuraAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.AbilityTag, SlotInputTag);
}

void USpellMenuWidgetController::DeselectSpellGlobe()
{
	const FGameplayTag& AbilityType = AbilityInformation->GetAbilityInfoByTag(SelectedAbility.AbilityTag, true).AbilityTypeTag;
	OnEndEquipSelection.Broadcast(AbilityType);
	bIsWaitingForEquipSelection = false;
	
	SelectedAbility.AbilityTag = Ability_None;
	SelectedAbility.StatusTag = Ability_Status_Locked;

	OnSpellGlobeSelected.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& InputTag, const FGameplayTag& OldInputTag)
{
	bIsWaitingForEquipSelection = false;

	FAuraAbilityInfo LastAuraAbilityInfo;
	LastAuraAbilityInfo.AbilityTag = Ability_None;
	LastAuraAbilityInfo.StatusTag = Ability_Status_Acquired;
	LastAuraAbilityInfo.InputTag = OldInputTag;
	AbilityInfoDelegate.Broadcast(LastAuraAbilityInfo);

	FAuraAbilityInfo Info = AbilityInformation->GetAbilityInfoByTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = InputTag;
	AbilityInfoDelegate.Broadcast(Info);

	OnEndEquipSelection.Broadcast(Info.AbilityTypeTag);
	OnSpellGlobeReassigned.Broadcast(AbilityTag);
	DeselectSpellGlobe();
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                     bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	
	if (AbilityStatus.MatchesTagExact(Ability_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(Ability_Status_Unlocked))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(Ability_Status_Acquired))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}

	/*switch (AbilityStatus)
	{
	case Ability_Status_Equipped:
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}

	case Ability_Status_Unlocked:
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}

	case Ability_Status_Acquired:
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}

	default:
		bShouldEnableSpellPointsButton = false;
		bShouldEnableEquipButton = false;	
	}*/
}
