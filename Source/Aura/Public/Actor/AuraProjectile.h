// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilitySystemTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;


UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAuraProjectile();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FAuraDamageEffectParams DamageEffectParams;

	UProjectileMovementComponent* GetProjectileMoveComp() const;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComp;

protected:
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnSphereCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMoveComp;

private:

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> Impact_VFX;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> Impact_SFX;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> Cast_SFX;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> OnAir_SFX;

	bool bHasServerHitHappened;
};
