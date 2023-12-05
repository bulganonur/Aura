// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FGameplayEffectSpec;
class UAbilitySystemComponent;

/** Variables in this delegate will also be available as output pin for the task's BP node */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownChangeSignature, float, TimeRemaining);

/**
 * AsyncTask will be an output pin for the BP node
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "WaitForCooldownChange", BlueprintInternalUseOnly = "TRUE"))
	static UWaitCooldownChange* CreateWaitCooldownChangeProxy(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

	/** Execution pin on BP node */
	UPROPERTY(BlueprintAssignable)
	FOnCooldownChangeSignature OnCooldownStart;

	/** Execution pin on BP node */
	UPROPERTY(BlueprintAssignable)
	FOnCooldownChangeSignature OnCooldownEnd;
	
protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;
	
	void OnCooldownTagChange(const FGameplayTag Tag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
