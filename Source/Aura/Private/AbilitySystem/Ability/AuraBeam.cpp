// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraBeam.h"

#include "AuraGameplayTags.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UAuraBeam::UAuraBeam()
{
	NumMaxJumpTargets = 5;
	NumJumpTargets = 1;
	ManaDrainAndDamageDeltaTime = 0.1f;
}

void UAuraBeam::CacheUnderCursorData(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		SetHitActor(HitResult.GetActor());
		SetHitLocation(HitResult.ImpactPoint);
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAuraBeam::CachePlayerController(APlayerController* PlayerController)
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
	}
}

void UAuraBeam::WeaponToCursorSphereTrace()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	const FVector CombatSocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(Avatar, CombatSocket_Weapon);
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(Avatar);
	FHitResult HitResult;

	UKismetSystemLibrary::SphereTraceSingle
	(
		Avatar,
		CombatSocketLocation,
		HitLocation,
		25.0f,
		TraceTypeQuery1,
		false,
		IgnoredActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		if (HitResult.GetActor()->Implements<UCombatInterface>())
		{
			SetCueTarget(HitResult.GetActor());
			SetHitActor(HitResult.GetActor());
			SetHitLocation(HitResult.ImpactPoint);
		}
		else
		{
			SetCueTarget(Avatar);
		}
	}
}

TArray<AActor*> UAuraBeam::SphereAreaTrace(AActor* OriginActor)
{
	if (!OriginActor) { return TArray<AActor*>(); }

	AActor* Avatar = GetAvatarActorFromActorInfo();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypesToFilter;
	ObjTypesToFilter.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(Avatar);
	IgnoredActors.Add(OriginActor);

	TArray<AActor*> HitActors;
	UKismetSystemLibrary::SphereOverlapActors
	(
		Avatar,
		OriginActor->GetActorLocation(),
		500.0f,
		ObjTypesToFilter,
		AAuraCharacterBase::StaticClass(),
		IgnoredActors,
		HitActors
	);

	/*DrawDebugSphere // SphereAreaTrace
	(
		GetWorld(),
		OriginActor->GetActorLocation(),
		500.0f,
		8,
		FColor::Magenta,
		false,
		5.0f,
		0,
		2.0f
	);*/

	if (!HitActors.IsEmpty())
	{
		/*for (const AActor* Actor : HitActors)
		{
			DrawDebugSphere
			(
				GetWorld(),
				Actor->GetActorLocation(),
				100.0f,
				12,
				FColor::Silver,
				false,
				5.0f,
				0,
				1.0f
			);
		}*/
		return HitActors;
	}
	return TArray<AActor*>();
}

AActor* UAuraBeam::GetNearestJumpTarget(const AActor* OriginActor, const TArray<AActor*>& JumpTargets)
{
	if (!OriginActor || JumpTargets.IsEmpty()) { return nullptr; }
	
	float MinDistance = OriginActor->GetDistanceTo(JumpTargets[0]);
	AActor* NearestActor = JumpTargets[0];
	for (int i = 1; i < JumpTargets.Num(); ++i)
	{
		const float Distance = OriginActor->GetDistanceTo(JumpTargets[i]);

		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestActor = JumpTargets[i];
		}
	}
	/*DrawDebugSphere
	(
		GetWorld(),
		NearestActor->GetActorLocation(),
		100.0f,
		24,
		FColor::Cyan,
		false,
		5.0f,
		0,
		1.0f
	);*/
	return NearestActor;
}

void UAuraBeam::AddToZappedActors(AActor* InActor)
{
	if (InActor)
	{
		ZappedActors.AddUnique(InActor);
	}
}

void UAuraBeam::SetTargets(AActor* InCueTarget, AActor* InHitActor, const FVector& InHitLocation)
{
	CueTarget = InCueTarget;
	HitActor = InHitActor;
	HitLocation = InHitLocation;
}

void UAuraBeam::SetCueTarget(AActor* InCueTarget)
{
	CueTarget = InCueTarget;
}

void UAuraBeam::SetHitActor(AActor* InHitActor)
{
	HitActor = InHitActor;
}

void UAuraBeam::SetHitLocation(const FVector& InHitLocation)
{
	HitLocation = InHitLocation;
}
