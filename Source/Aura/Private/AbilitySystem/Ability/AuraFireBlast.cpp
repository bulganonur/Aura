// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Ability/AuraFireBlast.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

UAuraFireBlast::UAuraFireBlast() : NumFireBalls(12)
{
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;
	
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	const FVector AvatarForward = Avatar->GetActorForwardVector();
	const FVector AvatarLocation = Avatar->GetActorLocation();
	
	AActor* Owner = GetOwningActorFromActorInfo();
	APawn* Pawn = GetCurrentActorInfo()->PlayerController->GetPawn();
	
	// TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(AvatarForward, FVector::UpVector, 360.0f, NumFireBalls);
	//
	// for (const FRotator& Rotator : Rotators)
	// {
	// 	FTransform SpawnTransform;
	// 	SpawnTransform.SetLocation(AvatarLocation);
	// 	SpawnTransform.SetRotation(Rotator.Quaternion());
	// 	
	// 	AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>
	// 	(
	// 		FireBallClass,
	// 		SpawnTransform,
	// 		Owner,
	// 		Pawn,
	// 		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	// 	);
	// 	
	// 	FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	// 	FireBalls.Add(FireBall);
	// 	FireBall->FinishSpawning(SpawnTransform);
	// }

	TArray<FVector> Vectors = UAuraAbilitySystemLibrary::EvenlySpacedVectors(AvatarForward, FVector::ZAxisVector, 360.0f, NumFireBalls);

	for (const FVector& Vector : Vectors)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(AvatarLocation);
		SpawnTransform.SetRotation(Vector.ToOrientationQuat());

		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>
		(
			FireBallClass,
			SpawnTransform,
			Owner,
			Pawn,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBalls.Add(FireBall);
		FireBall->FinishSpawning(SpawnTransform);
		
	}
	return FireBalls;
}
