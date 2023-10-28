// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	// this might be a better way to get the PlayerController for multiplayer
	/*APlayerController* PC = UWorld::GetFirstPlayerController();*/

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		AAuraHUD* AuraHUD = PC->GetHUD<AAuraHUD>();
		AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();

		if (AuraHUD && PS)
		{
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* ASet = PS->GetAttributeSet();
			const FWidgetControllerParams WCParams(PC, PS, ASC, ASet);
			
			return AuraHUD->GetOverlayWidgetController(WCParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	// this might be a better way to get the PlayerController for multiplayer
	/*APlayerController* PC = UWorld::GetFirstPlayerController();*/

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		AAuraHUD* AuraHUD = PC->GetHUD<AAuraHUD>();
		AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();

		if (AuraHUD && PS)
		{
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* ASet = PS->GetAttributeSet();
			const FWidgetControllerParams WCParams(PC, PS, ASC, ASet);
			
			return AuraHUD->GetAttributeMenuWidgetController(WCParams);
		}
	}
	return nullptr;
}
