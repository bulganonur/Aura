// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS()
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()

public:
	
	AMagicCircle();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> DecalComponent;

public:
	
	virtual void Tick(float DeltaTime) override;
	
};
