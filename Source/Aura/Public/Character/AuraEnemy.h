// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

enum class ECharacterClass : uint8;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	
	AAuraEnemy();

	//~ Begin Enemy Interface

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
	//~ End Enemy Interface

	//~ Begin Combat Interface

	virtual int32 GetAuraLevel() override;
	
	//~ End Combat Interface

protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangeSignature OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FOnAttributeChangeSignature OnMaxHealthChange;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> WidgetComp;

};
