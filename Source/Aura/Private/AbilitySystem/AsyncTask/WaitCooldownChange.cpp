// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTask/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::CreateWaitCooldownChangeProxy(UAbilitySystemComponent* AbilitySystemComponent,
                                                                        const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();
	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// To know when a Cooldown has ended (Cooldown Tag has been removed)
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag).AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnCooldownTagChange);

	// To know when a Cooldown GameplayEffect has been applied
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);
	
	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (!IsValid(ASC)) { return; }

	ASC->RegisterGameplayTagEvent(CooldownTag).RemoveAll(this);
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitCooldownChange::OnCooldownTagChange(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount == 0)
	{
		OnCooldownEnd.Broadcast(0.0f);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		/*OnCooldownStart.Broadcast(EffectSpec.Duration);*/

		const FGameplayEffectQuery EffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(CooldownTag.GetSingleTagContainer());
		const TArray<float> TimesRemaining = ASC->GetActiveEffectsTimeRemaining(EffectQuery);
		if (!TimesRemaining.IsEmpty())
		{
			// Get greatest value in the array
			float TimeRemaining = TimesRemaining[0];
			for (int i = 1; i < TimesRemaining.Num(); ++i)
			{
				if (TimeRemaining < TimesRemaining[i])
				{
					TimeRemaining = TimesRemaining[i];
				}
			}
			OnCooldownStart.Broadcast(TimeRemaining);
		}
	}
}
