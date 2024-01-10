// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UNiagaraComponent;
class AAuraPlayerState;

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	
	AAuraCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//~ Begin Player Interface

	virtual void AddToXP_Implementation(const int32 InXP) override;
	virtual void LevelUp_Implementation(const int32 InLevel) override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetLevelByXP_Implementation(const int32 InXP) override;
	virtual void AddToAttributePoints_Implementation(const int32 InPoints) override;
	virtual void AddToSpellPoints_Implementation(const int32 InPoints) override;
	
	//~ End Player Interface

	//~ Begin Combat Interface
	
	virtual int32 GetAuraLevel_Implementation() const override;
	
	//~ End Combat Interface

protected:
	
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated)
	bool bIsInShockLoop;

private:
	
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	TObjectPtr<UNiagaraComponent> VFX_LevelUp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComp;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUp();
	
};
