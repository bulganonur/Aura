// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// Super::BroadcastInitialValues();

	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		OnHealthChange.Broadcast(AuraAttributeSet->GetHealth(), AuraAttributeSet->GetHealth());
		OnMaxHealthChange.Broadcast(AuraAttributeSet->GetMaxHealth(), AuraAttributeSet->GetMaxHealth());
		OnManaChange.Broadcast(AuraAttributeSet->GetMana(), AuraAttributeSet->GetMana());
		OnMaxManaChange.Broadcast(AuraAttributeSet->GetMaxMana(), AuraAttributeSet->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// Super::BindCallbacksToDependencies();

	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnHealthChange.Broadcast(Data.OldValue, Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChange.Broadcast(Data.OldValue, Data.NewValue);
			});
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnManaChange.Broadcast(Data.OldValue, Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChange.Broadcast(Data.OldValue, Data.NewValue);
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
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			});
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
