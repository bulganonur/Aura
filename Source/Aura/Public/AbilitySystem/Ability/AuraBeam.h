// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraBeam.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeam : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:

	UAuraBeam();

	UFUNCTION(BlueprintCallable)
	void CacheUnderCursorData(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void CachePlayerController(APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable)
	void WeaponToCursorSphereTrace();

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> SphereAreaTrace(AActor* OriginActor);

	UFUNCTION(BlueprintCallable)
	AActor* GetNearestJumpTarget(const AActor* OriginActor, const TArray<AActor*>& JumpTargets);

	UFUNCTION(BlueprintCallable)
	void SetTargets(AActor* InCueTarget, AActor* InHitActor, const FVector& InHitLocation);

	UFUNCTION(BlueprintCallable)
	void AddToZappedActors(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	void SetCueTarget(AActor* InCueTarget);

	UFUNCTION(BlueprintCallable)
	void SetHitActor(AActor* InHitActor);

	UFUNCTION(BlueprintCallable)
	void SetHitLocation(const FVector& InHitLocation);

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> HitActor;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> CueTarget;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	TObjectPtr<UAnimMontage> Montage_Beam;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector HitLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	int32 NumJumpTargets;

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	int32 NumMaxJumpTargets;

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TArray<AActor*> ZappedActors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	float ManaDrainAndDamageDeltaTime;
};
