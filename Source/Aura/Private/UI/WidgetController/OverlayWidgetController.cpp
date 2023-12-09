// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// Super::BroadcastInitialValues();

	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChange.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChange.Broadcast(AuraAttributeSet->GetMaxHealth());
		OnManaChange.Broadcast(AuraAttributeSet->GetMana());
		OnMaxManaChange.Broadcast(AuraAttributeSet->GetMaxMana());
	}
	
	const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	if (!AuraPlayerState) { return; }

	OnXPChangeDelegate.Broadcast(AuraPlayerState->GetXP());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// Super::BindCallbacksToDependencies();

	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
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

		if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
		{
			if (AuraASC->bHasStartupAbilitiesGiven)
			{
				OnStartupAbilitiesGiven();
			}
			else
			{
				AuraASC->OnAbilityGivenDelegate.AddUObject(this, &ThisClass::OnStartupAbilitiesGiven);
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

			if (AAuraPlayerState* AuraPS = Cast<AAuraPlayerState>(PlayerState))
			{
				AuraPS->OnXPChangeDelegate.AddUObject(this, &ThisClass::OnXPChange);

				AuraPS->OnLevelChangeDelegate.AddLambda([this](const int32 StatValue)
				{
					OnAuraLevelChange.Broadcast(StatValue);
				});
			}
			
		}
	}
}

void UOverlayWidgetController::OnStartupAbilitiesGiven()
{
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		/*if (!AuraASC->bHasStartupAbilitiesGiven) { return; }*/

		FForEachAbility BroadcastDelegate;
		BroadcastDelegate.BindLambda([this, AuraASC](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAuraAbilityInfo Info = AbilityInfo->GetAbilityInfoByTag(AuraASC->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = AuraASC->GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});
		AuraASC->ForEachAbility(BroadcastDelegate);
	}
}

void UOverlayWidgetController::OnXPChange(const int32 NewXP) const
{
	// XPBarPercentage = CurrentXP - PreviousLevelUpReq / DeltaLevelUpReq

	const AAuraPlayerState* AuraPS = Cast<AAuraPlayerState>(PlayerState);
	if (!AuraPS) { return; }
	checkf(AuraPS->AuraLevelUpInfo, TEXT("AuraLevelUpInfo is not set! --> BP_AuraPlayerState"));
	
	const FAuraLevelUpInfo Info = AuraPS->AuraLevelUpInfo->GetLevelUpInfoByXP(NewXP);
	if (Info.Level > AuraPS->AuraLevelUpInfo->LevelUpInfo.Num() || Info.Level <= 0) { return; }
	
	const int32 CurrentLevelUpReq = AuraPS->AuraLevelUpInfo->LevelUpInfo[Info.Level].LevelUpRequirement;
	const int32 PreviousLevelUpReq = AuraPS->AuraLevelUpInfo->LevelUpInfo[Info.Level - 1].LevelUpRequirement;
	const int32 DeltaLevelUpReq = CurrentLevelUpReq - PreviousLevelUpReq;

	const float XPBarPercentage = static_cast<float>(NewXP - PreviousLevelUpReq) / static_cast<float>(DeltaLevelUpReq);

	// broadcast XPBarPercentage to registered widgets
	OnXPChangeDelegate.Broadcast(XPBarPercentage);
}
