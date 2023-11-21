// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

// Vital Attributes
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Vital_Health, "Attribute.Vital.Health", "Amount of Damage Taken Before Death");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Vital_Mana, "Attribute.Vital.Mana", "TBA");

// Primary Attributes
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Intelligence, "Attribute.Primary.Intelligence", "Increases Magical Damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Resilience, "Attribute.Primary.Resilience", "Increases Armor and Armor Penetration");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Strength, "Attribute.Primary.Strength", "Increases Physical Damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Primary_Vigor, "Attribute.Primary.Vigor", "Increases Health");

// Secondary Attributes
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_Armor, "Attribute.Secondary.Armor", "Reduces Damage Taken, Improves Block Chance");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_ArmorPenetration, "Attribute.Secondary.ArmorPenetration", "Ignores Percentage of Enemy Armor, Increases Critical Hit Chance");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_BlockChance, "Attribute.Secondary.BlockChance", "Chance to Cut the Damage in Half");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_CriticalHitChance, "Attribute.Secondary.CriticalHitChance", "Chance to Double the Damage Plus Critical Hit Bonus");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_CriticalHitDamage, "Attribute.Secondary.CriticalHitDamage", "Bonus Damage Added When a Critical Hit is Scored");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_CriticalHitResistance, "Attribute.Secondary.CriticalHitResistance", "Reduces Critical Hit Chance of Enemies");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_HealthRegeneration, "Attribute.Secondary.HealthRegeneration", "Amount of Health Regenerated Every Second");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_ManaRegeneration, "Attribute.Secondary.ManaRegeneration", "Amount of Mana Regenerated Every Second");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_MaxHealth, "Attribute.Secondary.MaxHealth", "Maximum Amount of Health Obtainable");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_MaxMana, "Attribute.Secondary.MaxMana", "Maximum Amount of Mana Obtainable");

// Secondary Attributes (Resistance Attributes)
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_ArcaneResistance, "Attribute.Secondary.ArcaneResistance", "Resistance to Arcane Damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_FireResistance, "Attribute.Secondary.FireResistance", "Resistance to Fire Damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_LightningResistance, "Attribute.Secondary.LightningResistance", "Resistance to Lightning Damage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Secondary_PhysicalResistance, "Attribute.Secondary.PhysicalResistance", "Resistance to Physical Damage");

// Messages
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message_HealthCrystal, "Message.HealthCrystal", "tba");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message_HealthPotion, "Message.HealthPotion", "tba");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message_ManaCrystal, "Message.ManaCrystal", "tba");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message_ManaPotion, "Message.ManaPotion", "tba");

// Input
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_LMB, "InputTag.LMB", "Input Tag for left mouse button");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_RMB, "InputTag.RMB", "Input Tag for right mouse button");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_1, "InputTag.1", "Input Tag for 1 key");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_2, "InputTag.2", "Input Tag for 2 key");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_3, "InputTag.3", "Input Tag for 3 key");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_4, "InputTag.4", "Input Tag for 4 key");

// Damage Types
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_DefaultType, "Damage.DefaultType", "Default Damage Type Tag. Usually for Set by Caller Magnitude");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Fire, "Damage.Fire", "Fire damage type");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Lightning, "Damage.Lightning", "Lightning damage type");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Arcane, "Damage.Arcane", "Arcane damage type");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Physical, "Damage.Physical", "Physical damage type");

// Effects
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_HitReact, "Effect.HitReact", "Tag Granted for hit reaction via GameplayEffect");

// Abilities
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Attack, "Ability.Attack", "Attack Ability");

// Montage
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_Weapon, "Montage.Attack.Weapon", "Weapon Attack Montage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_RightHand, "Montage.Attack.RightHand", "Right Hand Attack Montage");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Montage_Attack_LeftHand, "Montage.Attack.LeftHand", "Left Hand Attack Montage");

FAuraGameplayTags FAuraGameplayTags::AuraGameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// Filling GameplayTagContainer
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Vital_Health);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Vital_Mana);

	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Primary_Intelligence);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Primary_Resilience);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Primary_Strength);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Primary_Vigor);

	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_Armor);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_ArmorPenetration);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_BlockChance);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_CriticalHitChance);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_CriticalHitDamage);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_CriticalHitResistance);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_HealthRegeneration);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_ManaRegeneration);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_MaxHealth);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_MaxMana);

	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_ArcaneResistance);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_FireResistance);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_LightningResistance);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Attribute_Secondary_PhysicalResistance);

	// DamageTypes to Resistances Map
	AuraGameplayTags.DamageTypesToResistances.Add(Damage_Arcane, Attribute_Secondary_ArcaneResistance);
	AuraGameplayTags.DamageTypesToResistances.Add(Damage_Fire, Attribute_Secondary_FireResistance);
	AuraGameplayTags.DamageTypesToResistances.Add(Damage_Lightning, Attribute_Secondary_LightningResistance);
	AuraGameplayTags.DamageTypesToResistances.Add(Damage_Physical, Attribute_Secondary_PhysicalResistance);

}

const FAuraGameplayTags& FAuraGameplayTags::Get()
{
	return AuraGameplayTags;
}
