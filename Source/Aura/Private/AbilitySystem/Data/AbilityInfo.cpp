// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::GetAbilityInfoByTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& Info : AbilityInfo)
	{
		if (Tag.MatchesTagExact(Info.AbilityTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find Info for AbilityTag: [%s], on AbilityInfo: [%s]"), *Tag.ToString(), *GetNameSafe(this));
	}

	return FAuraAbilityInfo();
}
