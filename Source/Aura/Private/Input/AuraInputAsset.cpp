// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputAsset.h"

#include "InputAction.h"
#include "Aura/AuraLogChannels.h"

const UInputAction* UAuraInputAsset::GetAbilityInputActionByTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FAuraInputAction& Index : AbilityInputActions)
	{
		if (Tag == Index.InputTag && Index.InputAction)
		{
			return Index.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Input Action not found for InputTag: [%s], on AuraInputAsset: [%s]"), *Tag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
