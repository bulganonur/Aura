// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// Super::BroadcastInitialValues();

	if (GetAuraAttributeSet())
	{
		OnHealthChange.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChange.Broadcast(AuraAttributeSet->GetMaxHealth());
		OnManaChange.Broadcast(AuraAttributeSet->GetMana());
		OnMaxManaChange.Broadcast(AuraAttributeSet->GetMaxMana());
	}
	
	if (!GetAuraPlayerState()) { return; }
	OnXPChangeDelegate.Broadcast(AuraPlayerState->GetXP());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// Super::BindCallbacksToDependencies();

	if (GetAuraAttributeSet())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnHealthChange.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChange.Broadcast(Data.NewValue);
			});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnManaChange.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChange.Broadcast(Data.NewValue);
			});
	}
	
	if (GetAuraAbilitySystemComponent())
	{
		AuraASC->OnAbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		
		if (AuraASC->bHasStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			AuraASC->OnAbilityGivenDelegate.AddUObject(this, &ThisClass::BroadcastAbilityInfo);
		}
		
		AuraASC->EffectAssetTags.AddLambda([this](const FGameplayTagContainer& GEAssetTags)
		{
			for (const FGameplayTag& Tag : GEAssetTags)
			{
				// For example, Tag == Message.HealthPotion
				// "Message.HealthPotion".MatchesTag("Message") will return True,
				// "Message".MatchesTag("Message.HealthPotion") will return False
				
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag("Message");
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRow.Broadcast(*Row);
				}
			}
		});
	}
	
	if (GetAuraPlayerState())
	{
		AuraPlayerState->OnXPChangeDelegate.AddUObject(this, &ThisClass::OnXPChange);

		AuraPlayerState->OnLevelChangeDelegate.AddLambda([this](const int32 StatValue)
		{
			OnAuraLevelChange.Broadcast(StatValue);
		});
	}
}

void UOverlayWidgetController::OnXPChange(const int32 NewXP)
{
	// XPBarPercentage = CurrentXP - PreviousLevelUpReq / DeltaLevelUpReq
	
	if (!GetAuraPlayerState()) { return; }
	checkf(AuraPlayerState->AuraLevelUpInfo, TEXT("AuraLevelUpInfo is not set! --> BP_AuraPlayerState"));
	
	const FAuraLevelUpInfo Info = AuraPlayerState->AuraLevelUpInfo->GetLevelUpInfoByXP(NewXP);
	if (Info.Level > AuraPlayerState->AuraLevelUpInfo->LevelUpInfo.Num() || Info.Level <= 0) { return; }
	
	const int32 CurrentLevelUpReq = AuraPlayerState->AuraLevelUpInfo->LevelUpInfo[Info.Level].LevelUpRequirement;
	const int32 PreviousLevelUpReq = AuraPlayerState->AuraLevelUpInfo->LevelUpInfo[Info.Level - 1].LevelUpRequirement;
	const int32 DeltaLevelUpReq = CurrentLevelUpReq - PreviousLevelUpReq;

	const float XPBarPercentage = static_cast<float>(NewXP - PreviousLevelUpReq) / static_cast<float>(DeltaLevelUpReq);

	// broadcast XPBarPercentage to registered widgets
	OnXPChangeDelegate.Broadcast(XPBarPercentage);
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,
	const FGameplayTag& InputTag, const FGameplayTag& OldInputTag) const
{

	FAuraAbilityInfo LastAuraAbilityInfo;
	LastAuraAbilityInfo.AbilityTag = Ability_None;
	LastAuraAbilityInfo.StatusTag = Ability_Status_Acquired;
	LastAuraAbilityInfo.InputTag = OldInputTag;
	AbilityInfoDelegate.Broadcast(LastAuraAbilityInfo);

	FAuraAbilityInfo Info = AbilityInformation->GetAbilityInfoByTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = InputTag;
	AbilityInfoDelegate.Broadcast(Info);
	
}
