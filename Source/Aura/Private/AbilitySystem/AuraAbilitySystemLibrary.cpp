// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraAbilitySystemTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
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

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
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

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
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
			
			return AuraHUD->GetSpellMenuWidgetController(WCParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, float Level)
{
	if (UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		// this is a Data Asset(despite the naming)
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

void UAuraAbilitySystemLibrary::InitializeStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, int32 Level)
{
	if (UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterClassInfo->CommonAbilities)
		{
			FGameplayAbilitySpec GASpec = ASC->BuildAbilitySpecFromClass(AbilityClass, 1);
			ASC->GiveAbility(GASpec);
		}

		for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterClassInfo->GetClassDefaultInfo(CharacterClass).ClassAbilities)
		{
			FGameplayAbilitySpec GASpec = ASC->BuildAbilitySpecFromClass(AbilityClass, Level);
			ASC->GiveAbility(GASpec);
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	if (const AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return AuraGM->CharacterClassInfo;
	}
	return nullptr;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	if (const AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return AuraGM->AbilityInfo;
	}
	return nullptr;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& GEContextHandle, bool InIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(InIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& GEContextHandle, bool InIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(InIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& SphereOrigin,
	float Radius)
{
	FCollisionQueryParams SphereParams;
	SphereParams.bTraceComplex = false;
	SphereParams.MobilityType = EQueryMobilityType::Dynamic;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType
		(
			Overlaps,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams
		);
		if (Overlaps.IsEmpty()) { return; }
		for (const FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
	}
}

int32 UAuraAbilitySystemLibrary::GetXPRewardByClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, const int32 EnemyLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	return CharacterClassInfo ? CharacterClassInfo->GetClassDefaultInfo(CharacterClass).XPRewards.GetValueAtLevel(EnemyLevel) : 0;
}
