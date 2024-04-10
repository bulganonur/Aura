// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

class AAuraFireBall;
/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UAuraFireBlast();
	
protected:

	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();
	
	UPROPERTY(EditDefaultsOnly)
	int32 NumFireBalls;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraFireBall> FireBallClass;
};
