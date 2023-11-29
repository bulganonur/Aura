// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation, const FGameplayTag& SocketTag)
{
	/*const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();*/
	const bool bIsServer = HasAuthority(&GetCurrentActivationInfoRef());
	
	if (!bIsServer) { return; }
	
	const FVector SpawnLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	const FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());

	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>
	(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	// Setup GameplayEffect
	const UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle GEContextHandle = SourceASC->MakeEffectContext();
	GEContextHandle.SetAbility(this);
	GEContextHandle.AddSourceObject(Projectile);
	const FGameplayEffectSpecHandle GESpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), GEContextHandle);

	// Modify GameplayEffect - get all DamageTypes by tag and their magnitude then assign set by caller
	for (const auto& Map : DamageTypes)
	{
		const float ScaledDamage = Map.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GESpecHandle, Map.Key, ScaledDamage);
	}
	
	Projectile->DamageEffectSpecHandle = GESpecHandle;
	Projectile->FinishSpawning(SpawnTransform);
}
