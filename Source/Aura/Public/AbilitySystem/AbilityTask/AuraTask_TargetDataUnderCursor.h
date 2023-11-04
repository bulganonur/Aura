// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AuraTask_TargetDataUnderCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCursorTargetDataSignature, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class AURA_API UAuraTask_TargetDataUnderCursor : public UAbilityTask
{
	GENERATED_BODY()

public:
	/** When this function is called [usually in the editor], it sets the ability which calls this as its owner */
	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta = (DisplayName = "TargetDataUnderCursor", HidePin =  "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAuraTask_TargetDataUnderCursor* CreateTargetDataUnderCursorProxy(UGameplayAbility* OwningAbility);

	/** This will be an execution pin in the editor for the function TargetDataUnderCursor [CreateTargetDataUnderCursorProxy] */
	UPROPERTY(BlueprintAssignable)
	FCursorTargetDataSignature ValidData;

private:
	/** Called automatically when the owner ability's activation happens */
	virtual void Activate() override;

	/** Sends local/client data to server via server rpc and then broadcasts it to the ability listening [if still active] */
	void SendUnderCursorData();

	/** Called when the ability target data is set [AbilityTargetDataSet] */
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
