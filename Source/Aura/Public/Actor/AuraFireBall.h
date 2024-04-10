// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraProjectile.h"
#include "AuraFireBall.generated.h"

UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()

public:
	
	AAuraFireBall();
	virtual void Tick(float DeltaTime) override;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TravelDistance;
	
	UFUNCTION(BlueprintCallable)
	void FireBallInterp(const FVector& StartLocation, const FVector& EndLocation, float LerpAlpha);
	
	virtual void BeginPlay() override;
	virtual void OnSphereCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
