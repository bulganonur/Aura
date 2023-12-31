// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSpellGlobeSelectedSignature, bool, bSpellPointsButtonEnabled, bool, bEquipButtonEnabled, FString, CurrentLevelDesc, FString, NextLevelDesc);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipSelectionSignature, const FGameplayTag&, AbilityTypeTag);

struct FSelectedAbility
{
	FGameplayTag AbilityTag = FGameplayTag();
	FGameplayTag StatusTag = FGameplayTag();
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	USpellMenuWidgetController();

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Stats")
	FOnPlayerStatChange OnSpellPointChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnSpellGlobeSelectedSignature OnSpellGlobeSelected;

	UPROPERTY(BlueprintAssignable)
	FOnEquipSelectionSignature OnBeginEquipSelection;

	UPROPERTY(BlueprintAssignable)
	FOnEquipSelectionSignature OnEndEquipSelection;

	UPROPERTY(BlueprintAssignable)
	FOnEquipSelectionSignature OnSpellGlobeReassigned;
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void OnSpendSpellPointButtonPressed();

	UFUNCTION(BlueprintCallable)
	void OnEquipButtonPressed();

	UFUNCTION(BlueprintCallable)
	void OnEquippedSpellsRowPressed(const FGameplayTag& SlotInputTag, const FGameplayTag& AbilityTypeTag);

	UFUNCTION(BlueprintCallable)
	void DeselectSpellGlobe();

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& OldInputTag);

private:

	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility;
	int32 CurrentSpellPoints;
	bool bIsWaitingForEquipSelection;
	FGameplayTag SelectedSlotInputTag;
};
