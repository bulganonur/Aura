// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxMana.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	// Attribute to Capture
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();

	// Attribute Source
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// Snapshot
	IntelligenceDef.bSnapshot = false;

	// Add Intelligence to RelevantAttributes
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Get source and target Tags
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float IntelligenceMagnitude;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, OUT IntelligenceMagnitude);
	IntelligenceMagnitude = FMath::Max(IntelligenceMagnitude, 0.0f); // make sure Int is never less than 0.0f

	// check if the Source Object implements ICombatInterface
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	check(CombatInterface);
	
	const int32 AuraLevel = CombatInterface->GetAuraLevel();
	IntelligenceMagnitude = 50.0f + 2.5f * IntelligenceMagnitude + 15.0f * AuraLevel; // calculation
	
	return IntelligenceMagnitude;
}