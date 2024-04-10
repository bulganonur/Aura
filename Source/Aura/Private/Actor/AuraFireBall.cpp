// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/AuraFireBall.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AAuraFireBall::AAuraFireBall() : TravelDistance(500.0f)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraFireBall::FireBallInterp(const FVector& StartLocation, const FVector& EndLocation, float LerpAlpha)
{
	SetActorLocation(FMath::InterpExpoInOut(StartLocation, EndLocation, LerpAlpha));
}

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraFireBall::OnSphereCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator()) { return; }
	
	UGameplayStatics::PlaySoundAtLocation(this, Impact_SFX, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Impact_VFX, GetActorLocation());
	if (HasAuthority())
	{
		DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		DamageEffectParams.DeathImpulse = DamageEffectParams.DeathImpulseMagnitude * GetActorForwardVector();
		
		if (DamageEffectParams.SourceASC && DamageEffectParams.TargetASC)
		{
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
}

void AAuraFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

