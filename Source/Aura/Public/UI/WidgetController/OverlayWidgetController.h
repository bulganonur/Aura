// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAbilityInfo;
class UAuraUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChange, const float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfo, const FAuraAbilityInfo&, Info);

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRow, const FUIWidgetRow&, WidgetRow);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChange OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChange OnMaxHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChange OnManaChange;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChange OnMaxManaChange;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Messages")
	FMessageWidgetRow MessageWidgetRow;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Messages")
	FAbilityInfo AbilityInfoDelegate;

	/** fyi: XP is not an Attribute, using this signature for convenience */
	UPROPERTY(BlueprintAssignable, Category = "GAS | PlayerStats")
	FOnAttributeChange OnXPChangeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS | PlayerStats")
	FOnPlayerStatChange OnAuraLevelChange;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	void OnStartupAbilitiesGiven();
	void OnXPChange(const int32 NewXP) const;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
