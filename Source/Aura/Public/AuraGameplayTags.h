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
 * SECONDARY ATTRIBUTES (RESISTANCE ATTRIBUTES)
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_ArcaneResistance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_FireResistance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_LightningResistance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Secondary_PhysicalResistance);

/**
 * MESSAGES
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_HealthCrystal);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_HealthPotion);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_ManaCrystal);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Message_ManaPotion);

/**
 * INPUT
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4);

/**
 * DAMAGE TYPES
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_DefaultType);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Lightning);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Arcane);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical);

/**
 * EFFECTS
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_HitReact);

/**
 * AuraGameplayTags
 *
 * Singleton containing native GameplayTags
 * 
 */
struct FAuraGameplayTags
{

 static const FAuraGameplayTags& Get();
 static void InitializeNativeGameplayTags();

 FGameplayTagContainer AuraAllTagsContainer;
 TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
 
private:
 static FAuraGameplayTags AuraGameplayTags;
 
};
