// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilitySystemTypes.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/AuraLogChannels.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	bTopOffHealth = false;
	bTopOffMana = false;
	
	// Primary Attributes
	TagsToAttributes.Add(Attribute_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(Attribute_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(Attribute_Primary_Vigor, GetVigorAttribute);

	// Secondary Attributes
	TagsToAttributes.Add(Attribute_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(Attribute_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(Attribute_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(Attribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(Attribute_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(Attribute_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(Attribute_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(Attribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(Attribute_Secondary_MaxMana, GetMaxManaAttribute);

	// Secondary Attributes (Resistance Attributes)
	TagsToAttributes.Add(Attribute_Secondary_ArcaneResistance, GetArcaneResistanceAttribute);
	TagsToAttributes.Add(Attribute_Secondary_FireResistance, GetFireResistanceAttribute);
	TagsToAttributes.Add(Attribute_Secondary_LightningResistance, GetLightningResistanceAttribute);
	TagsToAttributes.Add(Attribute_Secondary_PhysicalResistance, GetPhysicalResistanceAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Secondary Attributes (Resistance Attributes)
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
		UE_LOG(LogAura, Warning, TEXT("PREHealth: %f"), NewValue);
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
		UE_LOG(LogAura, Warning, TEXT("PREMana: %f"), NewValue);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	if (ICombatInterface::Execute_IsDead(Props.TargetAvatar)) { return; }
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, OUT FEffectProperties& Props)
{
	// Source(Data.EffectSpec): Causer of the effect, Target(Data.Target): Owner of the AttributeSet(Data.EvaluatedData)

	Props.GEContextHandle = Data.EffectSpec.GetContext();

	// Source (Data.EffectSpec)
	Props.SourceASC = Props.GEContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (Props.SourceASC)
	{
		Props.SourceAvatar = Props.SourceASC->GetAvatarActor();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceAvatar)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceAvatar);
		}
	}

	// Target (Data.Target)
	Props.TargetAvatar = Data.Target.GetAvatarActor();
	Props.TargetASC = &Data.Target;
	Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
	if (Props.TargetAvatar)
	{
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatar);
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props) const
{
	const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetAvatar);
	const int32 TargetLevel = ICombatInterface::Execute_GetAuraLevel(Props.TargetAvatar);
	const int32 TargetXPReward = UAuraAbilitySystemLibrary::GetXPRewardByClassAndLevel(this, TargetClass, TargetLevel);

	FGameplayEventData Payload;
	Payload.EventTag = Attribute_Meta_IncomingXP;
	Payload.EventMagnitude = static_cast<float>(TargetXPReward);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceAvatar, Attribute_Meta_IncomingXP, Payload);
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.0f);

	if (LocalIncomingDamage > 0.0f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

		const bool bFatalDamage = NewHealth <= 0.0f;
		if (bFatalDamage)
		{
			if (ICombatInterface* EffectedTarget = Cast<ICombatInterface>(Props.TargetAvatar))
			{
				EffectedTarget->Die(UAuraAbilitySystemLibrary::GetDeathImpulse(Props.GEContextHandle));
			}
			SendXPEvent(Props);
		}
		else
		{
			const FGameplayTagContainer TagContainer = FGameplayTagContainer(Effect_HitReact);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
		}

		// Handle Debuff
		if (UAuraAbilitySystemLibrary::ShouldApplyDebuff(Props.GEContextHandle) && !bFatalDamage)
		{
			// AuraGameplayTags
			FAuraGameplayTags AuraTags = FAuraGameplayTags::Get();

			// Create new EffectContext for the Debuff
			FGameplayEffectContextHandle EffectContextHandle = Props.SourceASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(Props.SourceASC);

			// Get Debuff parameters
			const FGameplayTag& DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.GEContextHandle);
			const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.GEContextHandle);
			const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.GEContextHandle);
			const float DebuffPeriod = UAuraAbilitySystemLibrary::GetDebuffPeriod(Props.GEContextHandle);
			const FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());

			// Create and setup new GameplayEffect for Debuff
			UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));
			Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
			Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
			Effect->Period = DebuffPeriod;
			/**
			 * Note(for 5.3 and onwards):
			 * Inheritable Owned Tags Container is deprecated.
			 * To configure, add a UTargetTagsGameplayEffectComponent.
			 * To access, use GetGrantedTags.
			 * Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
			 * Fix:
			 * FInheritedTagContainer TagContainer = FInheritedTagContainer();
			 * UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
			 * TagContainer.Added.AddTag(AuraTags.DamageTypesToDebuffs[DamageType]);
			 * TagContainer.CombinedTags.AddTag(AuraTags.DamageTypesToDebuffs[DamageType]);
			 * Component.SetAndApplyTargetTagChanges(TagContainer);
			 */
			Effect->InheritableOwnedTagsContainer.AddTag(AuraTags.DamageTypesToDebuffs[DamageType]);
			Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
			Effect->StackLimitCount = 1;

			FGameplayModifierInfo ModifierInfo;
			ModifierInfo.Attribute = GetIncomingDamageAttribute();
			ModifierInfo.ModifierOp = EGameplayModOp::Additive;
			ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);

			Effect->Modifiers.Add(ModifierInfo);

			// Apply GameplayEffect
			if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContextHandle, 1.0f))
			{
				FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(MutableSpec->GetContext().Get());
				AuraEffectContext->SetDamageType(DamageType);

				Props.SourceASC->ApplyGameplayEffectSpecToTarget(*MutableSpec, Props.TargetASC);
			}
		}
			
		const bool bBlock = UAuraAbilitySystemLibrary::IsBlockedHit(Props.GEContextHandle);
		const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.GEContextHandle);
			
		if (AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(Props.SourceController))
		{
			AuraPC->ClientShowDamageWidget(Props.TargetAvatar, LocalIncomingDamage, bBlock, bCriticalHit);
			return; // return so widget is only shown once, for pvp
		}
		if (AAuraPlayerController* AuraPC = Cast<AAuraPlayerController>(Props.TargetController))
		{
			AuraPC->ClientShowDamageWidget(Props.TargetAvatar, LocalIncomingDamage, bBlock, bCriticalHit);
		}
	}
}

void UAuraAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.0f);
		
	const int32 CurrentLevel = ICombatInterface::Execute_GetAuraLevel(Props.SourceAvatar);
	const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceAvatar);
	const int32 NewLevel = IPlayerInterface::Execute_GetLevelByXP(Props.SourceAvatar, CurrentXP + LocalIncomingXP);
	const int32 LevelUps = NewLevel - CurrentLevel;

	// @todo: try Execute_AddToXP (or any other PlayerInterface function) with an object which does not implements IPlayerInterface
	IPlayerInterface::Execute_AddToXP(Props.SourceAvatar, LocalIncomingXP);
		
	if (LevelUps > 0)
	{
		IPlayerInterface::Execute_LevelUp(Props.SourceAvatar, LevelUps);
		bTopOffHealth = true;
		bTopOffMana = true;
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}
