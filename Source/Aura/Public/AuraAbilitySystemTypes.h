// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilitySystemTypes.generated.h"

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
	void SetIsBlockedHit(const bool InIsBlockedHit) { bIsBlockedHit = InIsBlockedHit; }
	void SetIsCriticalHit(const bool InIsCriticalHit) { bIsCriticalHit = InIsCriticalHit; }
	
protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
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
