// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveSpellNiagara.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UPassiveSpellNiagara : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	UPassiveSpellNiagara();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveAbilityTag;
	
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bShouldActivate);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
