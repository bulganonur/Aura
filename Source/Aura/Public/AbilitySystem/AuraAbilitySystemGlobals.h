// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 *
 * Don't forget to add this to DefaultGame.ini file
 * 
 * [/Script/GameplayAbilities.AbilitySystemGlobals]
 * +AbilitySystemGlobalsClassName="/Script/Aura.AuraAbilitySystemGlobals"
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:

	/** Should allocate a project specific GameplayEffectContext struct. Caller is responsible for deallocation */
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
