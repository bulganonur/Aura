// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AuraBTS_FindNearestPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void UAuraBTS_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerComp.GetAIOwner()) { return; }
	const APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!AIPawn && !BlackboardComponent) { return; }
	
	const FName TargetTag = AIPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	// Query the World and get Actors with the Tag(not GameplayTag, just FNames)
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(AIPawn, TargetTag, ActorsWithTag);
	
	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	for (AActor* Actor : ActorsWithTag)
	{
		if (IsValid(Actor) && IsValid(AIPawn))
		{
			const float Distance = AIPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}

	BlackboardComponent->SetValueAsObject(TargetActorSelector.SelectedKeyName, ClosestActor);
	BlackboardComponent->SetValueAsFloat(DistanceToTargetSelector.SelectedKeyName, ClosestDistance);
	/*UBTFunctionLibrary::SetBlackboardValueAsObject(this,TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);*/
}
