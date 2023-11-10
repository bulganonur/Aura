// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>("WidgetComp");
	WidgetComp->SetupAttachment(GetRootComponent());
	
	Level = 1;
	bHitReacting = false;
	BaseWalkSpeed = 250.0f;
	AfterDeathLifeSpan = 5.0f;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	UAuraAbilitySystemLibrary::InitializeStartupAbilities(this, AbilitySystemComponent);

	// set progressbar's widget controller as this
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(WidgetComp->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}
	
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		// bind callbacks/lambdas to dependencies
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnHealthChange.Broadcast(Data.OldValue, Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChange.Broadcast(Data.OldValue, Data.NewValue);
			});

		// broadcast initial values
		OnHealthChange.Broadcast(AuraAttributeSet->GetHealth(), AuraAttributeSet->GetHealth());
		OnMaxHealthChange.Broadcast(AuraAttributeSet->GetMaxHealth(), AuraAttributeSet->GetMaxHealth());

		AbilitySystemComponent->RegisterGameplayTagEvent(Effect_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnHitReactTagChange);
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->PostInitAbilityActorInfo();

	InitializeDefaultAttributes();
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, AbilitySystemComponent, CharacterClass, Level);
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetAuraLevel() const
{
	return Level;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(AfterDeathLifeSpan);
	Super::Die();
}

void AAuraEnemy::OnHitReactTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.0f : BaseWalkSpeed;
}
