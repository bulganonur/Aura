// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& TargetLocation);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
};