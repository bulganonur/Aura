// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Get AbilitySystemComponents
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// Get AvatarActors
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	// Get CombatInterfaces
	const ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	const ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	// Get GameplayEffectSpec
	const FGameplayEffectSpec& GESpec = ExecutionParams.GetOwningSpec();

	// Get GameplayEffectContextHandle
	FGameplayEffectContextHandle GEContextHandle = GESpec.GetContext();
	
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = GESpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = GESpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Actual damage of the effect
	float Damage = 0.0f;
	
	// Get Damage Set by Caller Magnitude - get all damage types and add their values to Damage
	for (const FGameplayTag& Tag : FAuraGameplayTags::Get().DamageTypes)
	{
		const float DamageTypeValue = GESpec.GetSetByCallerMagnitude(Tag);
		Damage += DamageTypeValue;
	}
	
	
	// Get target's BlockChance magnitude
	float TargetBlockChance;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, OUT TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.0f);
	
	// Check if the hit was a block, if so halve the Damage
	const bool bBlocked = FMath::RandRange(1.0f, 100.0f) <= TargetBlockChance;
	Damage = bBlocked ? Damage * 0.5 : Damage;

	// Set IsBlockedHit
	UAuraAbilitySystemLibrary::SetIsBlockedHit(GEContextHandle, bBlocked);
	
	// Get target's Armor Magnitude
	float TargetArmor;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, OUT TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.0f);

	// Get source's ArmorPenetration Magnitude
	float SourceArmorPenetration;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, OUT SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.0f);

	// Get ArmorPenetration Coefficient from the CurveTable
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const UCurveTable* CoefficientsCT = CharacterClassInfo->DamageCalculationCoefficients;
	const FRealCurve* ArmorPenetrationCurve = CoefficientsCT->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetAuraLevel());
	
	// ArmorPenetration ignores a percentage of the target's Armor
	const float EffectiveArmor = TargetArmor * (100.0f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.0f;

	// Get EffectiveArmor Coefficient from the CurveTable
	const FRealCurve* EffectiveArmorCurve = CoefficientsCT->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetAuraLevel());
	
	// Armor ignores a percentage of the IncomingDamage
	Damage *= (100.0f - EffectiveArmor * EffectiveArmorCoefficient) / 100.0f;

	// Get source's CriticalHitChance Magnitude
	float SourceCriticalHitChance;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, OUT SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.0f);
	
	// Get source's CriticalHitDamage Magnitude
	float SourceCriticalHitDamage;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, OUT SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.0f);

	// Get source's CriticalHitDamage Magnitude
	float TargetCriticalHitResistance;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, OUT TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.0f);

	// Get CriticalHitResistance Coefficient from the CurveTable
	const FRealCurve* CriticalHitResistanceCurve = CoefficientsCT->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetAuraLevel());
	
	// CriticalHitResistance reduces CriticalHitChance
	const float EffectiveCriticalHitChance = SourceCriticalHitChance * (100.0f - TargetCriticalHitResistance * CriticalHitResistanceCoefficient) / 100.0f;

	// Double damage plus bonus (CriticalHitDamage), if critical hit
	bool bCriticalHit = FMath::RandRange(1.0f, 100.0f) <= EffectiveCriticalHitChance;
	Damage = bCriticalHit ? Damage * 2.0f + SourceCriticalHitDamage : Damage;

	//Set IsCriticalHit
	UAuraAbilitySystemLibrary::SetIsCriticalHit(GEContextHandle, bCriticalHit);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

float UExecCalc_Damage::GetCapturedAttributeMagnitude(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParams, const FGameplayEffectAttributeCaptureDefinition& CaptureDefinition) const
{
	float AttributeMagnitude;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvaluateParams, AttributeMagnitude);
	AttributeMagnitude = FMath::Max<float>(AttributeMagnitude, 0.0f);

	return AttributeMagnitude;
}
