// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FGameplayTag;
struct FAuraDamageEffectParams;
class UAbilityInfo;
class USpellMenuWidgetController;
class UCharacterClassInfo;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
enum class ECharacterClass : uint8;
struct FGameplayEffectContextHandle;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, float Level);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | CharacterClassDefaults")
	static void InitializeStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, int32 Level);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | AbilityInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static bool ShouldApplyDebuff(const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static float GetDebuffDamage (const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static float GetDebuffDuration (const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static float GetDebuffPeriod (const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static FGameplayTag GetDamageType (const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static FVector GetDeathImpulse (const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static FVector GetKnockbackForce (const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& GEContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, const bool InIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, const bool InIsCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetShouldApplyDebuff(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, const bool InShouldApplyDebuff);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, const float InDebuffDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, const float InDebuffDuration);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetDebuffPeriod(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, const float InDebuffPeriod);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, const FGameplayTag& InDamageTypeTag);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, const FVector& InDeathImpulse);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& GEContextHandle, const FVector& InKnockbackForce);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetIsRadialDamage(FGameplayEffectContextHandle& GEContextHandle, const bool InIsRadialDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetRadialDamageInnerRadius(FGameplayEffectContextHandle& GEContextHandle, const float InRadialDamageInnerRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetRadialDamageOuterRadius(FGameplayEffectContextHandle& GEContextHandle, const float InRadialDamageOuterRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static void SetRadialDamageOrigin(FGameplayEffectContextHandle& GEContextHandle, const FVector& InRadialDamageOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& SphereOrigin, float Radius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | CharacterClassDefaults")
	static int32 GetXPRewardByClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, const int32 EnemyLevel);

	static TArray<FRotator> EvenlySpacedRotators(const FVector& ForwardVector, const FVector& Axis, float SpreadAngle, int32 SpreadNum);
	static TArray<FVector> EvenlySpacedVectors(const FVector& ForwardVector, const FVector& Axis, float SpreadAngle, int32 SpreadNum);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary | GameplayEffects")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FAuraDamageEffectParams& EffectParams);
};
