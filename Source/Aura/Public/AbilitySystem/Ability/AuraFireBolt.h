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

	UAuraFireBolt();
	
	virtual FString GetDescriptionByLevel(const int32 AbilityLevel) override;
	virtual FString GetDescriptionByNextLevel(const int32 AbilityLevel) override;

protected:
	
	UFUNCTION(BlueprintCallable, Category = "FireBolt")
	void SpawnFireBolt(const AActor* HomingTarget, const FVector& TargetLocation, const FGameplayTag& SocketTag);

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	int32 MaxNumProjectiles;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ProjectileSpread;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float MinAcceleration;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float MaxAcceleration;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float PitchOverride;
};
