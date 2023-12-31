// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraFireBolt.h"
#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescriptionByLevel(const int32 AbilityLevel)
{
	const int32 Damage = GetDamageByDamageType(Damage_Fire, AbilityLevel);
	const float ManaCost = GetManaCostByLevel(AbilityLevel);
	const float Cooldown = GetCooldownByLevel(AbilityLevel);
	
	if (AbilityLevel == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>"
			"\n\n"
			"<Small>Level:</><Level>%i</>" // AbilityLevel
			"\n"
			"<Small>ManaCost:</><ManaCost>%.1f</>" // ManaCost
			"\n"
			"<Small>Cooldown:</><Cooldown>%.1f</>" // Cooldown
			"\n\n"
			"<Default>Launches a bolt of fire, exploding on impact and dealing </>"
			"<Damage>%i</><Default> fire damage with a chance to burn</>" // Damage
			), AbilityLevel, FMath::Abs(ManaCost), Cooldown, Damage);
	}
	return FString::Printf(TEXT(
		"<Title>FIRE BOLT</>"
		"\n\n"
		"<Small>Level:</><Level>%i</>" // AbilityLevel
		"\n"
		"<Small>ManaCost:</><ManaCost>%.1f</>" // ManaCost
		"\n"
		"<Small>Cooldown:</><Cooldown>%.1f</>" // Cooldown
		"\n\n"
		"<Default>Launches %i bolts of fire, exploding on impact and dealing </>" // NumProjectiles
		"<Damage>%i</><Default> fire damage with a chance to burn</>" // Damage
		), AbilityLevel, FMath::Abs(ManaCost), Cooldown, FMath::Min(AbilityLevel, NumProjectiles), Damage);
}

FString UAuraFireBolt::GetDescriptionByNextLevel(const int32 AbilityLevel)
{
	const int32 Damage = GetDamageByDamageType(Damage_Fire, AbilityLevel);
	const float ManaCost = GetManaCostByLevel(AbilityLevel);
	const float Cooldown = GetCooldownByLevel(AbilityLevel);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL</>"
		"\n\n"
		"<Small>Level:</><Level>%i</>" // AbilityLevel
		"\n"
		"<Small>ManaCost:</><ManaCost>%.1f</>" // ManaCost
		"\n"
		"<Small>Cooldown:</><Cooldown>%.1f</>" // Cooldown
		"\n\n"
		"<Default>Launches %i bolts of fire, exploding on impact and dealing </>" // NumProjectiles
		"<Damage>%i</><Default> fire damage with a chance to burn</>" // Damage
		), AbilityLevel, FMath::Abs(ManaCost), Cooldown, FMath::Min(AbilityLevel, NumProjectiles), Damage);
}