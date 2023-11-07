// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
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

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, float Level)
{
	if (const AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		// this is a Data Asset(despite the naming)
		UCharacterClassInfo* CharacterClassInfo = AuraGM->CharacterClassInfo;
		const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

		const AActor* Avatar = ASC->GetAvatarActor();

		FGameplayEffectContextHandle GEContextHandle_Primary = ASC->MakeEffectContext();
		GEContextHandle_Primary.AddSourceObject(Avatar);
		const FGameplayEffectSpecHandle GESpecHandle_Primary = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, GEContextHandle_Primary);
		ASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle_Primary.Data.Get());

		FGameplayEffectContextHandle GEContextHandle_Secondary = ASC->MakeEffectContext();
		GEContextHandle_Secondary.AddSourceObject(Avatar);
		const FGameplayEffectSpecHandle GESpecHandle_Secondary = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, GEContextHandle_Secondary);
		ASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle_Secondary.Data.Get());

		FGameplayEffectContextHandle GEContextHandle_Vital = ASC->MakeEffectContext();
		GEContextHandle_Vital.AddSourceObject(Avatar);
		const FGameplayEffectSpecHandle GESpecHandle_Vital = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, GEContextHandle_Vital);
		ASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle_Vital.Data.Get());
	}
}
