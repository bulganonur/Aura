// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	virtual FString GetDescriptionByLevel(const int32 AbilityLevel);
	virtual FString GetDescriptionByNextLevel(const int32 AbilityLevel);
	static FString GetLockedDescription(const int32 Level);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

protected:

	float GetManaCostByLevel(const float InLevel) const;
	float GetCooldownByLevel(const float InLevel) const;
	
};


