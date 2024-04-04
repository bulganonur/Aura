// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Passive/PassiveSpellNiagara.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"


// Sets default values for this component's properties
UPassiveSpellNiagara::UPassiveSpellNiagara()
{
	bAutoActivate = false;
	
}


// Called when the game starts
void UPassiveSpellNiagara::BeginPlay()
{
	Super::BeginPlay();

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraASC->ActivatePassiveAbility.AddUObject(this, &UPassiveSpellNiagara::OnPassiveActivate);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC))
			{
				AuraASC->ActivatePassiveAbility.AddUObject(this, &UPassiveSpellNiagara::OnPassiveActivate);
			}
		});
	}
}

// Called every frame
void UPassiveSpellNiagara::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UPassiveSpellNiagara::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bShouldActivate)
{
	if (AbilityTag.MatchesTagExact(PassiveAbilityTag))
	{
		if (bShouldActivate && !IsActive())
		{
			Activate();
		}
		else if (!bShouldActivate && IsActive())
		{
			Deactivate();
		}
	}
}
