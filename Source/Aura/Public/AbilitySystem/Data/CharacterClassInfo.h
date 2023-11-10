// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

/**
 * @todo: better to replace this with GameplayTags.
 */
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger
};

/**
 *
 * If this is not populated further (Currently only one variable), can be moved in to the data asset class.
 * If you decide to move don't forget to update the TMap and the getter function.
 * 
 */
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
};

/**
 * DataAsset for enemy Attributes and Abilities
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Common for all enemies */
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	/** Common for all enemies */
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	/** Enums to GameplayEffects */
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	/** Common for all enemies */
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults | Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;
	
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
