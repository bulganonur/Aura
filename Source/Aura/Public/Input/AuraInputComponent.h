// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputAsset.h"
#include "AuraInputComponent.generated.h"


/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputAsset* InputAsset, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputAsset* InputAsset, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputAsset)

	for (const FAuraInputAction& Index : InputAsset->AbilityInputActions)
	{
		if (Index.InputAction && Index.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Index.InputAction, ETriggerEvent::Started, Object, PressedFunc, Index.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Index.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Index.InputTag);
			}
			
			if (HeldFunc)
			{
				BindAction(Index.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Index.InputTag);
			}
		}
	}
}
