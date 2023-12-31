// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

public:

	virtual FString GetDescriptionByLevel(const int32 AbilityLevel) override;
	virtual FString GetDescriptionByNextLevel(const int32 AbilityLevel) override;
};
