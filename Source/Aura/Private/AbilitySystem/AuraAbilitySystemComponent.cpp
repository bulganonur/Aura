// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::PostInitAbilityActorInfo()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnGameplayEffectApplied);
}

void UAuraAbilitySystemComponent::OnGameplayEffectApplied(UAbilitySystemComponent* ASC,
const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle GEHandle)
{
	FGameplayTagContainer TagContainer;
	GESpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
	

	
}
