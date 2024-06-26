// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/AuraProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	LifeSpan = 15.0f;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionObjectType(ECC_Projectile);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMoveComp->InitialSpeed = 550.0f;
	ProjectileMoveComp->MaxSpeed = 550.0f;
	ProjectileMoveComp->ProjectileGravityScale = 0.0f;

	OnAir_SFX = CreateDefaultSubobject<UAudioComponent>("OnAir_SFX");
	OnAir_SFX->SetupAttachment(GetRootComponent());

	bHasServerHitHappened = false;
}

UProjectileMovementComponent* AAuraProjectile::GetProjectileMoveComp() const
{
	return ProjectileMoveComp;
}

void AAuraProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereCompBeginOverlap);
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	SetLifeSpan(LifeSpan);
	UGameplayStatics::PlaySoundAtLocation(this, Cast_SFX, GetActorLocation());
}

void AAuraProjectile::Destroyed()
{
	// if Destroy called before OnSphereCompBeginOverlap for the Client, play visual and sound effects
	if (!bHasServerHitHappened && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, Impact_SFX, GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Impact_VFX, GetActorLocation());
		bHasServerHitHappened = true;
	}

	Super::Destroyed();
}

void AAuraProjectile::OnSphereCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator()) { return; }
	
	if (!bHasServerHitHappened)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Impact_SFX, GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Impact_VFX, GetActorLocation());
		bHasServerHitHappened = true;
	}
	
	if (HasAuthority()) // for server, carry on. everything happens as expected
	{
		DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		DamageEffectParams.DeathImpulse = DamageEffectParams.DeathImpulseMagnitude * GetActorForwardVector();
		
		if (DamageEffectParams.SourceASC && DamageEffectParams.TargetASC)
		{
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
	else // for client, set hit to true in case Destroy is called before Overlap
	{
		bHasServerHitHappened = true;
	}
}



