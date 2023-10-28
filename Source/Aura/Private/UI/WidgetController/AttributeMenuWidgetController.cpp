// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// Super::BroadcastInitialValues();

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	for (const auto& Map : AuraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Map.Key, Map.Value());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// Super::BindCallbacksToDependencies();

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	for (const auto& Map : AuraAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			Map.Value()).AddLambda([this, Map](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Map.Key, Map.Value());
			});
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const
{
	check(AttributeInfo);
	
	FAuraAttributeInfo Info = AttributeInfo->GetAttributeInfoByTag(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	OnAttributeInfoChangeDelegate.Broadcast(Info);
}
