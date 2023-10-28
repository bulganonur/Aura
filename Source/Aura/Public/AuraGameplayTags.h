// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * VITAL ATTRIBUTES
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Vital_Health);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Vital_Mana);

/**
 * PRIMARY ATTRIBUTES
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Intelligence);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Resilience);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Strength);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Primary_Vigor);

/**
 * SECONDARY ATTRIBUTES
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_Armor);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_ArmorPenetration);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_BlockChance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_CriticalHitChance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_CriticalHitDamage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_CriticalHitResistance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_HealthRegeneration);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_ManaRegeneration);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_MaxHealth);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_MaxMana);

/**
 * MESSAGES
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_HealthCrystal);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_HealthPotion);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_ManaCrystal);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_ManaPotion);

/**
 * AuraGameplayTags
 *
 * Singleton containing native GameplayTags
 * 
 */
struct FAuraGameplayTags
{
public:
 static const FAuraGameplayTags& Get();
 
 static void InitializeNativeGameplayTags();

 FGameplayTagContainer AuraAllTagsContainer;
 FGameplayTagContainer AttributeVitalContainer;
 FGameplayTagContainer AttributePrimaryContainer;
 FGameplayTagContainer AttributeSecondaryContainer;

protected:

 
private:
 static FAuraGameplayTags AuraGameplayTags;
 
};
