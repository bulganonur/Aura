// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

FAuraDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FAuraDamageEffectParams Params;
	
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.SourceASC = GetAbilitySystemComponentFromActorInfo();
	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.DamageEffectClass = DamageEffectClass;
	Params.DamageType = DamageType;
	Params.AbilityLevel = GetAbilityLevel();
	Params.Damage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffPeriod = DebuffPeriod;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;
	return Params;
}

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	// Setup GameplayEffect
	const FGameplayEffectSpecHandle& GESpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());

	// AssignTagSetByCaller
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GESpecHandle, DamageType, ScaledDamage);
	
	// Apply GameplayEffect
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}
