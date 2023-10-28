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

// Messages
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message_HealthCrystal, "Message.HealthCrystal", "tba");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message_HealthPotion, "Message.HealthPotion", "tba");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message_ManaCrystal, "Message.ManaCrystal", "tba");
UE_DEFINE_GAMEPLAY_TAG_COMMENT(Message_ManaPotion, "Message.ManaPotion", "tba");

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

	AuraGameplayTags.AuraAllTagsContainer.AddTag(Message_HealthCrystal);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Message_HealthPotion);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Message_ManaCrystal);
	AuraGameplayTags.AuraAllTagsContainer.AddTag(Message_ManaPotion);
}

const FAuraGameplayTags& FAuraGameplayTags::Get()
{
	return AuraGameplayTags;
}
