// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(GetRootComponent());
	SpringArmComp->SetUsingAbsoluteRotation(true);
	SpringArmComp->bDoCollisionTest = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
	
	VFX_LevelUp = CreateDefaultSubobject<UNiagaraComponent>("VFX_Niagara");
	VFX_LevelUp->SetupAttachment(GetRootComponent());
	VFX_LevelUp->SetAutoActivate(false);
	VFX_LevelUp->SetUsingAbsoluteRotation(true);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();

	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AAuraCharacter::AddToXP_Implementation(const int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation(const int32 InLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	const int32 CurrentLevel = AuraPlayerState->GetAuraLevel();
	int32 AttributePointReward = 0;
	int32 SpellPointReward = 0;
	for (int i = CurrentLevel + 1; i <= CurrentLevel + InLevel; ++i)
	{
		AttributePointReward += AuraPlayerState->AuraLevelUpInfo->LevelUpInfo[i].AttributePointReward;
		SpellPointReward += AuraPlayerState->AuraLevelUpInfo->LevelUpInfo[i].SpellPointReward;
	}
	
	AuraPlayerState->AddToLevel(InLevel);
	AuraPlayerState->AddToAttributePoints(AttributePointReward);
	AuraPlayerState->AddToSpellPoints(SpellPointReward);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->SetAbilityStatusTagsByLevel(CurrentLevel + InLevel);
	}

	MulticastLevelUp();
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::GetLevelByXP_Implementation(const int32 InXP)
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->AuraLevelUpInfo->GetLevelUpInfoByXP(InXP).Level;
}

void AAuraCharacter::AddToAttributePoints_Implementation(const int32 InPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddToAttributePoints(InPoints);
}

void AAuraCharacter::AddToSpellPoints_Implementation(const int32 InPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddToSpellPoints(InPoints);
}

int32 AAuraCharacter::GetAuraLevel_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAuraLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	// Set ASC & Attribute Set
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// Broadcast ASC is valid
	OnAscRegisteredDelegate.Broadcast(AbilitySystemComponent);

	// Initialize Ability Actor Info and Call PostInit
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent());
	AuraASC->InitAbilityActorInfo(AuraPlayerState, this);
	AuraASC->PostInitAbilityActorInfo();
	
	// Initialize Widget Overlay
	if (AAuraPlayerController* AuraPlayerController = GetController<AAuraPlayerController>())
	{
		if (AAuraHUD* AuraHUD = AuraPlayerController->GetHUD<AAuraHUD>())
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	// Initialize Default Attributes
	InitializeDefaultAttributes();
}

void AAuraCharacter::MulticastLevelUp_Implementation()
{
	if (!VFX_LevelUp) { return; }
	/*VFX_LevelUp->SetWorldRotation(GetViewRotation());*/
	VFX_LevelUp->Activate(true);
}
