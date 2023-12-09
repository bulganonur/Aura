// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangeSignature, const int32/** StatValue */);

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> AuraLevelUpInfo;
	
	int32 GetAuraLevel() const;
	int32 GetXP() const;
	int32 GetAttributePoints() const;
	int32 GetSpellPoints() const;

	void SetLevel(const int32 InLevel);
	void SetXP(const int32 InXP);
	void SetAttributePoints(const int32 InAttributePoints);
	void SetSpellPoints(const int32 InSpellPoints);

	void AddToLevel(const int32 InLevel);
	void AddToXP(const int32 InXP);
	void AddToAttributePoints(const int32 InAttributePoints);
	void AddToSpellPoints(const int32 InSpellPoints);

	FOnPlayerStatChangeSignature OnXPChangeDelegate;
	FOnPlayerStatChangeSignature OnLevelChangeDelegate;
	FOnPlayerStatChangeSignature OnAttributePointChangeDelegate;
	FOnPlayerStatChangeSignature OnSpellPointChangeDelegate;
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints;

	UFUNCTION()
	void OnRep_Level(const int32 OldLevel) const;

	UFUNCTION()
	void OnRep_XP(const int32 OldXP) const;

	UFUNCTION()
	void OnRep_AttributePoints(const int32 OldAttributePoints);

	UFUNCTION()
	void OnRep_SpellPoints(const int32 OldSpellPoints);
};
