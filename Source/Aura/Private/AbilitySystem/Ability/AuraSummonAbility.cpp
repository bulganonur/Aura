// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations() const
{
	TArray<FVector> SpawnLocations;

	const FVector AvatarForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector AvatarLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = (SpawnSpread / MinionNum) * 2; // Angle between each spread

	const FVector LeftSpread = AvatarForward.RotateAngleAxis(-SpawnSpread, FVector::ZAxisVector); // Initial spread/angle
	for (int i = 0; i <= MinionNum; ++i)
	{
		const FVector Spread = LeftSpread.RotateAngleAxis(DeltaSpread * i, FVector::ZAxisVector);
		FVector RandSpawnLocation = AvatarLocation + Spread * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, RandSpawnLocation + FVector(0.0f, 0.0f, 500.0f), RandSpawnLocation - FVector(0.0, 0.0, 500.0f), ECC_Visibility))
		{
			RandSpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(RandSpawnLocation);

		// Draw Debugs
		/*UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), AvatarLocation, AvatarLocation + Spread * MaxSpawnDistance, 4.0f, FLinearColor::Blue, 5.0f, 5.0f);
		DrawDebugSphere(GetWorld(), AvatarLocation + Spread * MinSpawnDistance, 15.f, 12, FColor::Magenta, false, 5.0f);
		DrawDebugSphere(GetWorld(), AvatarLocation + Spread * MaxSpawnDistance, 15.f, 12, FColor::Magenta, false, 5.0f);
		DrawDebugSphere(GetWorld(), RandSpawnLocation, 30.f, 15, FColor::Silver, false, 5.0f, 0, 3.0f);*/
	}
	
	return SpawnLocations;
}
