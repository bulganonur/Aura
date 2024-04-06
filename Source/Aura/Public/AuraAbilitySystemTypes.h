// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilitySystemTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FAuraDamageEffectParams
{
	GENERATED_BODY()

	FAuraDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;
	
	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	
	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float AbilityLevel = 1.0f;
	
	UPROPERTY()
	float Damage = 0.0f;

	UPROPERTY()
	float DebuffChance = 0.0f;

	UPROPERTY()
	float DebuffDamage = 0.0f;

	UPROPERTY()
	float DebuffDuration = 0.0f;

	UPROPERTY()
	float DebuffPeriod = 0.0f;

	UPROPERTY()
	float DeathImpulseMagnitude = 0.0f;
	
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	float KnockbackForceMagnitude = 0.0f;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY()
	float KnockbackChance = 0.0f;

	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialDamageInnerRadius = 0.0f;

	UPROPERTY()
	float RadialDamageOuterRadius = 0.0f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

/**
 * Custom GameplayEffectContext struct
 */
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override;

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const override;

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool ShouldApplyDebuff() const { return bShouldApplyDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffPeriod() const { return DebuffPeriod; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return  RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	
	void SetIsBlockedHit(const bool InIsBlockedHit) { bIsBlockedHit = InIsBlockedHit; }
	void SetIsCriticalHit(const bool InIsCriticalHit) { bIsCriticalHit = InIsCriticalHit; }
	void SetShouldApplyDebuff(const bool InShouldApplyDebuff) { bShouldApplyDebuff = InShouldApplyDebuff; }
	void SetDebuffDamage(const float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	void SetDebuffDuration(const float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	void SetDebuffPeriod(const float InDebuffPeriod) { DebuffPeriod = InDebuffPeriod; }
	void SetDamageType(const FGameplayTag& InDamageTypeTag);
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	void SetKnockbackForce(const FVector& InKnockbackForce) { KnockbackForce = InKnockbackForce; }
	void SetIsRadialDamage(const bool InIsRadialDamage) { bIsRadialDamage = InIsRadialDamage; }
	void SetRadialDamageInnerRadius(const float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius(const float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bShouldApplyDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.0f;

	UPROPERTY()
	float DebuffDuration = 0.0f;

	UPROPERTY()
	float DebuffPeriod = 0.0f;
	
	TSharedPtr<FGameplayTag> DamageType = nullptr;

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialDamageInnerRadius = 0.0f;

	UPROPERTY()
	float RadialDamageOuterRadius = 0.0f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
