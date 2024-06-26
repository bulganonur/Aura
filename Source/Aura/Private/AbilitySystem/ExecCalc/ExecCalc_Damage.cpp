// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);

		TagsToCaptureDefs.Add(Attribute_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Attribute_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(Attribute_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(Attribute_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Attribute_Secondary_CriticalHitResistance, CriticalHitResistanceDef);

		TagsToCaptureDefs.Add(Attribute_Secondary_ArcaneResistance, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Attribute_Secondary_FireResistance, FireResistanceDef);
		TagsToCaptureDefs.Add(Attribute_Secondary_LightningResistance, LightningResistanceDef);
		TagsToCaptureDefs.Add(Attribute_Secondary_PhysicalResistance, PhysicalResistanceDef);
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

	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Get AbilitySystemComponents
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// Get AvatarActors
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	if (!SourceASC || !TargetASC) { return; }
	
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

	// Debuff
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	for (TTuple<FGameplayTag, FGameplayTag> Pair : AuraTags.DamageTypesToDebuffs)
	{
		float EffectMagnitude = GESpec.GetSetByCallerMagnitude(Pair.Key, false, -1.0f);
		if (EffectMagnitude > -0.5f) // .5 padding for floating point [im]precision
		{
			const float SourceDebuffChance = GESpec.GetSetByCallerMagnitude(Debuff_Chance, false, -1);

			const FGameplayTag& ResistanceTag = AuraTags.DamageTypesToResistances[Pair.Key];
			float TargetDebuffResistance;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TagsToCaptureDefs[ResistanceTag], EvaluationParameters, OUT TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max(0.0f, TargetDebuffResistance);

			const float EffectiveDebuffChance = SourceDebuffChance * (100.0f - TargetDebuffResistance) / 100.0f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				UAuraAbilitySystemLibrary::SetShouldApplyDebuff(GEContextHandle, true);
				UAuraAbilitySystemLibrary::SetDamageType(GEContextHandle, Pair.Key);

				const float DebuffDamage = GESpec.GetSetByCallerMagnitude(Debuff_Damage, false, -1);
				const float DebuffDuration = GESpec.GetSetByCallerMagnitude(Debuff_Duration, false, -1);
				const float DebuffPeriod = GESpec.GetSetByCallerMagnitude(Debuff_Period, false, -1);

				UAuraAbilitySystemLibrary::SetDebuffDamage(GEContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(GEContextHandle, DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffPeriod(GEContextHandle, DebuffPeriod);
			}
		}
	}
	
	// Actual damage of the effect
	float Damage = 0.0f;
	
	// Get all damage types(and their SetByCallerMagnitudes) and resistances(associated with DamageTypes) of this effect
	// for the calculation of the actual Damage
	for (const TTuple<FGameplayTag, FGameplayTag>& Map : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Map.Key;

		// Get Resistance Tag associated with the Damage Type Tag
		const FGameplayTag ResistanceTag = Map.Value;

		// check if the CaptureDef exists for this Resistance Tag
		checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in Class: [%s] !"), *ResistanceTag.ToString(), *GetNameSafe(this));

		// Get Capture Definition associated with the Resistance Tag
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics().TagsToCaptureDefs[ResistanceTag];

		// Get the actual damage set by the caller for this DamageType Tag
		float DamageTypeValue = GESpec.GetSetByCallerMagnitude(Map.Key, false);

		// Get Target's Resistance magnitude
		float TargetResistance;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, OUT TargetResistance);
		TargetResistance = FMath::Max<float>(TargetResistance, 0.0f);

		// Ignore some of the Damage based on Target's Resistance
		DamageTypeValue *= (100.0f - TargetResistance) / 100.0f;
		if (DamageTypeValue <= 0.0)
		{
			continue;
		}

		if (UAuraAbilitySystemLibrary::IsRadialDamage(GEContextHandle))
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetOnDamageDelegate().AddLambda([&](float DamageAmount)
				{
					DamageTypeValue = DamageAmount;
				});
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff
			(
				TargetAvatar,
				DamageTypeValue,
				0.0f,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(GEContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(GEContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(GEContextHandle),
				1.0f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar
			);
		}

		// Add the resulting DamageTypeValue to the actual damage of this Effect
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
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(ICombatInterface::Execute_GetAuraLevel(SourceAvatar));
	
	// ArmorPenetration ignores a percentage of the target's Armor
	const float EffectiveArmor = TargetArmor * (100.0f - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.0f;

	// Get EffectiveArmor Coefficient from the CurveTable
	const FRealCurve* EffectiveArmorCurve = CoefficientsCT->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(ICombatInterface::Execute_GetAuraLevel(TargetAvatar));
	
	// Armor ignores a percentage of the IncomingDamage
	Damage *= (100.0f - EffectiveArmor * EffectiveArmorCoefficient) / 100.0f;

	// Get source's CriticalHitChance Magnitude
	float SourceCriticalHitChance;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, OUT SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.0f);
	
	// Get source's CriticalHitDamage Magnitude
	float SourceCriticalHitDamage;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, OUT SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.0f);

	// Get target's CriticalHitResistance Magnitude
	float TargetCriticalHitResistance;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, OUT TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.0f);

	// Get CriticalHitResistance Coefficient from the CurveTable
	const FRealCurve* CriticalHitResistanceCurve = CoefficientsCT->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(ICombatInterface::Execute_GetAuraLevel(TargetAvatar));
	
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
	// @todo: use this getter in Execute_Implementation function and refactor further if possible
	
	float AttributeMagnitude;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition, EvaluateParams, AttributeMagnitude);
	AttributeMagnitude = FMath::Max<float>(AttributeMagnitude, 0.0f);

	return AttributeMagnitude;
}
