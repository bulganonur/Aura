// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"

UAuraFireBolt::UAuraFireBolt()
{
	MaxNumProjectiles = 5;
	ProjectileSpread = 45.0f;
	MinAcceleration = 1000.0f;
	MaxAcceleration = 2000.0f;
	PitchOverride = 45.0f;
}

FString UAuraFireBolt::GetDescriptionByLevel(const int32 AbilityLevel)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(AbilityLevel);
	const float ManaCost = GetManaCostByLevel(AbilityLevel);
	const float Cooldown = GetCooldownByLevel(AbilityLevel);
	
	if (AbilityLevel == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>"
			"\n\n"
			"<Small>Level:</><Level>%i</>" // AbilityLevel
			"\n"
			"<Small>ManaCost:</><ManaCost>%.1f</>" // ManaCost
			"\n"
			"<Small>Cooldown:</><Cooldown>%.1f</>" // Cooldown
			"\n\n"
			"<Default>Launches a bolt of fire, exploding on impact and dealing </>"
			"<Damage>%i</><Default> fire damage with a chance to burn</>" // Damage
			), AbilityLevel, FMath::Abs(ManaCost), Cooldown, ScaledDamage);
	}
	return FString::Printf(TEXT(
		"<Title>FIRE BOLT</>"
		"\n\n"
		"<Small>Level:</><Level>%i</>" // AbilityLevel
		"\n"
		"<Small>ManaCost:</><ManaCost>%.1f</>" // ManaCost
		"\n"
		"<Small>Cooldown:</><Cooldown>%.1f</>" // Cooldown
		"\n\n"
		"<Default>Launches %i bolts of fire, exploding on impact and dealing </>" // NumProjectiles
		"<Damage>%i</><Default> fire damage with a chance to burn</>" // Damage
		), AbilityLevel, FMath::Abs(ManaCost), Cooldown, FMath::Min(AbilityLevel, MaxNumProjectiles), ScaledDamage);
}

FString UAuraFireBolt::GetDescriptionByNextLevel(const int32 AbilityLevel)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(AbilityLevel);
	const float ManaCost = GetManaCostByLevel(AbilityLevel);
	const float Cooldown = GetCooldownByLevel(AbilityLevel);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL</>"
		"\n\n"
		"<Small>Level:</><Level>%i</>" // AbilityLevel
		"\n"
		"<Small>ManaCost:</><ManaCost>%.1f</>" // ManaCost
		"\n"
		"<Small>Cooldown:</><Cooldown>%.1f</>" // Cooldown
		"\n\n"
		"<Default>Launches %i bolts of fire, exploding on impact and dealing </>" // NumProjectiles
		"<Damage>%i</><Default> fire damage with a chance to burn</>" // Damage
		), AbilityLevel, FMath::Abs(ManaCost), Cooldown, FMath::Min(AbilityLevel, MaxNumProjectiles), ScaledDamage);
}

void UAuraFireBolt::SpawnFireBolt(const AActor* HomingTarget, const FVector& TargetLocation, const FGameplayTag& SocketTag)
{
	/*const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();*/
	const bool bIsServer = HasAuthority(&GetCurrentActivationInfoRef());
	if (!bIsServer) { return; }

	const FVector SpawnLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	const FRotator SpawnRotation = (TargetLocation - SpawnLocation).ToOrientationRotator();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation); // SpawnLocation
	
	NumProjectiles = FMath::Min(GetAbilityLevel(), MaxNumProjectiles);
	const FVector SpawnForwardUnit = SpawnRotation.Vector();
	TArray<FRotator> Spreads = UAuraAbilitySystemLibrary::EvenlySpacedRotators(SpawnForwardUnit, FVector::UnitZ(), ProjectileSpread, NumProjectiles);
	
	for (const FRotator& Spread : Spreads)
	{
		SpawnTransform.SetRotation((Spread + FRotator(PitchOverride, 0.0f, 0.0f)).Quaternion()); // SpawnRotation

		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>
		(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
	
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		// Homing Setup
		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->GetProjectileMoveComp()->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComp = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComp->SetWorldLocation(TargetLocation);
			Projectile->GetProjectileMoveComp()->HomingTargetComponent = Projectile->HomingTargetSceneComp;
		}
		Projectile->GetProjectileMoveComp()->HomingAccelerationMagnitude = FMath::FRandRange(MinAcceleration, MaxAcceleration);
		Projectile->GetProjectileMoveComp()->bIsHomingProjectile = true;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
