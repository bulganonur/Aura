// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle& GESpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());

	// Setup GameplayEffect
	for (const auto& Map : DamageTypes)
	{
		const float ScaledDamage = Map.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GESpecHandle, Map.Key, ScaledDamage);
	}

	// Apply GameplayEffect
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

float UAuraDamageGameplayAbility::GetDamageByDamageType(const FGameplayTag& DamageTypeTag, const float InLevel) const
{
	return DamageTypes[DamageTypeTag].GetValueAtLevel(InLevel);
}
