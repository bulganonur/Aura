// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAuraUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangeSignature, float, OldValue, float, NewValue);

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, const FUIWidgetRow&, WidgetRow);

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
	FOnAttributeChangeSignature OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangeSignature OnMaxHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangeSignature OnManaChange;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangeSignature OnMaxManaChange;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	
}
