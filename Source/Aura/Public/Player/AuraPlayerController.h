// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputAsset;
class IEnemyInterface;
class UInputAction;
class UInputMappingContext;
struct FGameplayTag;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ClientShowDamageWidget(AActor* TargetActor, float DamageValue, bool bBlockedHit, bool bCriticalHit);

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	void Move(const FInputActionValue& InputActionValue);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LShiftAction;

	void LShiftPressed();
	void LShiftReleased();

	bool bLShiftKeyPressed;
	
	void CursorTrace();

	FHitResult CursorHit;
	IEnemyInterface* LastActor;
	IEnemyInterface* CurrentActor;

	// @todo: test TScriptInterface 
	// TScriptInterface<IEnemyInterface> LastActor;
	// TScriptInterface<IEnemyInterface> CurrentActor;
	
	void AbilityInputTagPressed(FGameplayTag Tag);
	void AbilityInputTagReleased(FGameplayTag Tag);
	void AbilityInputTagHeld(FGameplayTag Tag);
	

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputAsset> AuraInputAsset;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraASC;

	UAuraAbilitySystemComponent* GetAbilitySystemComponent();

	FVector CachedDestination;
	float FollowTime;
	float ShortPressThreshold;
	bool bAutoRunning;
	bool bTargetingEnemy;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AutoRunAcceptanceRadius;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
