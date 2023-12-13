// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// Super::BroadcastInitialValues();

	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	for (const auto& Map : AuraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Map.Key, Map.Value());
	}

	const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	if (!AuraPlayerState) { return; }
	
	OnAttributePointChange.Broadcast(AuraPlayerState->GetAttributePoints());
	OnSpellPointChange.Broadcast(AuraPlayerState->GetSpellPoints());
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

	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	if (!AuraPlayerState) { return; }

	AuraPlayerState->OnAttributePointChangeDelegate.AddLambda([this](const int32 StatValue)
	{
		OnAttributePointChange.Broadcast(StatValue);
	});

	AuraPlayerState->OnSpellPointChangeDelegate.AddLambda([this](const int32 StatValue)
	{
		OnSpellPointChange.Broadcast(StatValue);
	});
	
}

void UAttributeMenuWidgetController::SetAttributeValueByTag(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!AuraASC) { return; }

	AuraASC->SetAttributeValueByTag(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const
{
	check(AttributeInfo);
	
	FAuraAttributeInfo Info = AttributeInfo->GetAttributeInfoByTag(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	OnAttributeInfoChangeDelegate.Broadcast(Info);
}
