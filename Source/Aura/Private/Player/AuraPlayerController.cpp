// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.0f;
	ShortPressThreshold = 0.5f;
	bAutoRunning = false;
	AutoRunAcceptanceRadius = 50.f;
	bTargetingEnemy = false;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Hand;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	AuraInputComponent->BindAbilityActions(AuraInputAsset, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisValue = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ForwardVector, InputAxisValue.Y);
		ControlledPawn->AddMovementInput(RightVector, InputAxisValue.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) { return; }

	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	/**
	 *
	 * Line Trace from cursor. There are several scenarios:
	 * 
	 *  A. LastActor is null && CurrentActor is null.
	 *		- Do nothing
	 *	B. LastActor is null && CurrentActor is valid.
	 *		- Highlight CurrentActor
	 *	C. LastActor is valid && CurrentActor is null.
	 *		- UnHighlight LastActor
	 *	D. Both Actors are valid, but LastActor != CurrentActor
	 *		- UnHighlight LastActor and then Highlight CurrentActor
	 *	E. Both Actors are valid, and LastActor == CurrentActor
	 *		- Do nothing
	 */
	
	if (LastActor != CurrentActor)
	{
		if (CurrentActor) { CurrentActor->HighlightActor(); }
		if (LastActor) { LastActor->UnHighlightActor(); }
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) { return; }
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector PawnToSplineDirection = Spline->FindDirectionClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(PawnToSplineDirection);
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination < AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag Tag)
{
	if (Tag.MatchesTagExact(InputTag_LMB))
	{
		bTargetingEnemy = CurrentActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag Tag)
{
	if (!Tag.MatchesTagExact(InputTag_LMB) && GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->AbilityInputTagReleased(Tag);
		return;
	}

	if (bTargetingEnemy && GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->AbilityInputTagReleased(Tag);
	}
	else
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{

			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& Vector : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(Vector, ESplineCoordinateSpace::World);
					DrawDebugSphere
					(
						GetWorld(),
						Vector,
						8.0f,
						16,
						FColor::Purple,
						false,
						5.0f,
						0,
						2.0f
					);
				}
				CachedDestination = NavPath->PathPoints.IsEmpty() ? CachedDestination : NavPath->PathPoints.Last();
				bAutoRunning = NavPath->PathPoints.IsEmpty() ? false : true;
			}	
		}
		FollowTime = 0.0f;
		bTargetingEnemy = false;
	}
	
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag Tag)
{
	if (!Tag.MatchesTagExact(InputTag_LMB) && GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->AbilityInputTagHeld(Tag);
		return;
	}

	if (bTargetingEnemy && GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->AbilityInputTagHeld(Tag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		bAutoRunning = false; // ?
		
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.Location;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAbilitySystemComponent()
{
	if (!AuraASC)
	{
		AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}

	return AuraASC;
}
