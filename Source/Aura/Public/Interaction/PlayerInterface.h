// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(const int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetLevelByXP(const int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(const int32 InPoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(const int32 InPoints);
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp(const int32 InLevel);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();
};
