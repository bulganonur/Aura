// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Level = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointReward = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SpellPointReward = 1;
};

/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	
	FAuraLevelUpInfo GetLevelUpInfoByXP(const int32 XP) const;
	FAuraLevelUpInfo GetLevelUpInfoByLevel(const int32 Level, bool bLogNotFound = false) const;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty ="{Level}"))
	TArray<FAuraLevelUpInfo> LevelUpInfo;
};
