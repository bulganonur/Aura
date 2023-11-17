// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AuraBTS_FindNearestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBTS_FindNearestPlayer : public UBTService
{
	GENERATED_BODY()

protected:

	/**
	 * update next tick interval
	 * this function should be considered as const (don't modify state of object) if node is not instanced!
	 * bNotifyTick must be set to true for this function to be called 
	 * Calling INIT_SERVICE_NODE_NOTIFY_FLAGS in the constructor of the service will set this flag automatically
	 */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorSelector;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;
	
};
