// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilitySystemTypes.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:

	FAuraDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffChance;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffDuration;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffPeriod;

	UPROPERTY(EditDefaultsOnly, Category = "oomph")
	float DeathImpulseMagnitude;

	UPROPERTY(EditDefaultsOnly, Category = "oomph")
	float KnockbackChance;

	UPROPERTY(EditDefaultsOnly, Category = "oomph")
	float KnockbackForceMagnitude;
	
};
