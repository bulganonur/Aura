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
 * META ATTRIBUTES
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Meta_IncomingDamage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Meta_IncomingXP);

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

UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Default);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Passive_1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Passive_2);

/**
 * DAMAGE TYPES
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_DefaultType);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Lightning);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Arcane);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical);

/**
 * DEBUFFS
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Default);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Burn);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Stun);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Arcane);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Physical);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Chance);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Damage);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Duration);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Debuff_Period);

/**
 * EFFECTS
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_HitReact);

/**
 * ABILITIES
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Default);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_None);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Summon);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Fire_FireBolt);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Lightning_Beam);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_HitReact);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Status_Default);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Status_Locked);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Status_Unlocked);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Status_Acquired);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Status_Equipped);

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Default);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_None);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Offensive);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Type_Passive);

/**
 * COMBAT SOCKETS
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Weapon);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_RightHand);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_LeftHand);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Tail);

/**
 * MONTAGE TAGS
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_1);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_2);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_3);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_4);

/**
 * COOL DOWNS
 */

UE_DECLARE_GAMEPLAY_TAG_EXTERN(CoolDown_Fire_FireBolt);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(CoolDown_Lightning_Beam);

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
 TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
 
private:
 static FAuraGameplayTags AuraGameplayTags;
 
};
