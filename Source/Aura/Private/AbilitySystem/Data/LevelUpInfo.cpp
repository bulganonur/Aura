// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelUpInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraLevelUpInfo ULevelUpInfo::GetLevelUpInfoByXP(const int32 XP) const
{
	if (LevelUpInfo.Num() < 2) return FAuraLevelUpInfo();
	
	for (int i = 1; i < LevelUpInfo.Num(); ++i)
	{
		if (XP <= LevelUpInfo[i].LevelUpRequirement && XP > LevelUpInfo[i - 1].LevelUpRequirement)
		{
			return LevelUpInfo[i];
		}
	}
	
	return FAuraLevelUpInfo();
}

FAuraLevelUpInfo ULevelUpInfo::GetLevelUpInfoByLevel(const int32 Level, bool bLogNotFound) const
{
	if (Level < LevelUpInfo.Num())
	{
		return LevelUpInfo[Level];
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("No such Level exists! Level: %i, Check: %s DataAsset."), Level, *GetNameSafe(this));
	}

	return FAuraLevelUpInfo();
}
