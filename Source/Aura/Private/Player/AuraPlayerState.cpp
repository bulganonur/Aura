// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.0f;
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	Level = 1;
	XP = 0;
	AttributePoints = 0;
	SpellPoints = 0;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

int32 AAuraPlayerState::GetAuraLevel() const
{
	return Level;
}

void AAuraPlayerState::SetLevel(const int32 InLevel)
{
	if (InLevel >= 0)
	{
		Level = InLevel;
		OnLevelChangeDelegate.Broadcast(Level);
	}
}

void AAuraPlayerState::AddToLevel(const int32 InLevel)
{
	if (InLevel > 0)
	{
		Level += InLevel;
		OnLevelChangeDelegate.Broadcast(Level);
	}
}

int32 AAuraPlayerState::GetXP() const
{
	return XP;
}

int32 AAuraPlayerState::GetAttributePoints() const
{
	return AttributePoints;
}

int32 AAuraPlayerState::GetSpellPoints() const
{
	return SpellPoints;
}

void AAuraPlayerState::SetXP(const int32 InXP)
{
	if (InXP >= 0)
	{
		XP = InXP;
		OnXPChangeDelegate.Broadcast(XP);
	}
}

void AAuraPlayerState::SetAttributePoints(const int32 InAttributePoints)
{
	AttributePoints = InAttributePoints;
	OnAttributePointChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::SetSpellPoints(const int32 InSpellPoints)
{
	SpellPoints = InSpellPoints;
	OnSpellPointChangeDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::AddToXP(const int32 InXP)
{
	if (InXP > 0)
	{
		XP += InXP;
		OnXPChangeDelegate.Broadcast(XP);
	}
}

void AAuraPlayerState::AddToAttributePoints(const int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToSpellPoints(const int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnSpellPointChangeDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::OnRep_Level(const int32 OldLevel) const
{
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_XP(const int32 OldXP) const
{
	OnXPChangeDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(const int32 OldAttributePoints)
{
	OnAttributePointChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(const int32 OldSpellPoints)
{
	OnSpellPointChangeDelegate.Broadcast(SpellPoints);
}