// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilitySystemTypes.h"
#include "AuraGameplayTags.h"
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

bool UAuraAbilitySystemLibrary::ShouldApplyDebuff(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->ShouldApplyDebuff();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDamage();
	}
	return 0.0f;
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDuration();
	}
	return 0.0f;
}

float UAuraAbilitySystemLibrary::GetDebuffPeriod(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffPeriod();
	}
	return 0.0f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return *AuraEffectContext->GetDamageType().Get();
	}
	return FGameplayTag();
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UAuraAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool UAuraAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->IsRadialDamage();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.0f;
}

float UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.0f;
}

FVector UAuraAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& GEContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& GEContextHandle, const bool InIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(InIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& GEContextHandle, const bool InIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(InIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::SetShouldApplyDebuff(FGameplayEffectContextHandle& GEContextHandle, const bool InShouldApplyDebuff)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetShouldApplyDebuff(InShouldApplyDebuff);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& GEContextHandle, const float InDebuffDamage)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffDamage(InDebuffDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& GEContextHandle, const float InDebuffDuration)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffPeriod(FGameplayEffectContextHandle& GEContextHandle, const float InDebuffPeriod)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffPeriod(InDebuffPeriod);
	}
}

void UAuraAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& GEContextHandle, const FGameplayTag& InDamageTypeTag)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetDamageType(InDamageTypeTag);
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& GEContextHandle, const FVector& InDeathImpulse)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& GEContextHandle, const FVector& InKnockbackForce)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetKnockbackForce(InKnockbackForce);
	}
}

void UAuraAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& GEContextHandle, const bool InIsRadialDamage)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetIsRadialDamage(InIsRadialDamage);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& GEContextHandle, const float InRadialDamageInnerRadius)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageInnerRadius(InRadialDamageInnerRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& GEContextHandle, const float InRadialDamageOuterRadius)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageOuterRadius(InRadialDamageOuterRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& GEContextHandle, const FVector& InRadialDamageOrigin)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(GEContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageOrigin(InRadialDamageOrigin);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& SphereOrigin, float Radius)
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

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotators(const FVector& ForwardVector, const FVector& Axis, float SpreadAngle, int32 SpreadNum)
{
	TArray<FRotator> Spreads;
	if (SpreadNum > 1)
	{
		const float DeltaSpread = SpreadAngle / (SpreadNum - 1); // Angle between each spread
		const FVector LeftSpread = ForwardVector.RotateAngleAxis(-SpreadAngle * 0.5f, Axis); // Initial spread/angle
		for (int i = 0; i < SpreadNum; ++i)
		{
			const FVector Spread = LeftSpread.RotateAngleAxis(DeltaSpread * i, FVector::ZAxisVector);
			Spreads.Add(Spread.ToOrientationRotator());
		}
	}
	else
	{
		Spreads.Add(ForwardVector.ToOrientationRotator());
	}
	return Spreads;
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlySpacedVectors(const FVector& ForwardVector, const FVector& Axis, float SpreadAngle, int32 SpreadNum)
{
	TArray<FVector> Spreads;
	if (SpreadNum > 1)
	{
		const float DeltaSpread = SpreadAngle / (SpreadNum - 1); // Angle between each spread
		const FVector LeftSpread = ForwardVector.RotateAngleAxis(-SpreadAngle * 0.5f, Axis); // Initial spread/angle
		for (int i = 0; i < SpreadNum; ++i)
		{
			const FVector Spread = LeftSpread.RotateAngleAxis(DeltaSpread * i, FVector::ZAxisVector);
			Spreads.Add(Spread);
		}
	}
	else
	{
		Spreads.Add(ForwardVector);
	}
	return Spreads;
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FAuraDamageEffectParams& EffectParams)
{
	// Setup GameplayEffect
	FGameplayEffectContextHandle GEContextHandle = EffectParams.SourceASC->MakeEffectContext();
	GEContextHandle.AddSourceObject(EffectParams.SourceASC);
	SetDeathImpulse(GEContextHandle, EffectParams.DeathImpulse);
	const FGameplayEffectSpecHandle GESpecHandle = EffectParams.SourceASC->MakeOutgoingSpec(EffectParams.DamageEffectClass, EffectParams.AbilityLevel, GEContextHandle);

	// Radial Damage
	SetIsRadialDamage(GEContextHandle, EffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(GEContextHandle, EffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(GEContextHandle, EffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(GEContextHandle, EffectParams.RadialDamageOrigin);
	
	// SetByCallerMagnitude
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GESpecHandle, EffectParams.DamageType, EffectParams.Damage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GESpecHandle, Debuff_Chance, EffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GESpecHandle, Debuff_Damage, EffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GESpecHandle, Debuff_Duration, EffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GESpecHandle, Debuff_Period, EffectParams.DebuffPeriod);
	
	EffectParams.SourceASC->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data, EffectParams.TargetASC);

	return GEContextHandle;
}
