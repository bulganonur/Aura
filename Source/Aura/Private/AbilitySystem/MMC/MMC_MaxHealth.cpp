// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// Attribute to Capture
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();

	// Attribute Source
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// Snapshot
	VigorDef.bSnapshot = false;

	// Add Vigor to RelevantAttributes
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float VigorMagnitude;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, OUT VigorMagnitude);
	VigorMagnitude = FMath::Max<float>(VigorMagnitude, 0.0f); // Make sure Vigor is never below 0.0f

	// check if the Source Object implements ICombatInterface and get AuraLevel
	const int32 AuraLevel = ICombatInterface::Execute_GetAuraLevel(Spec.GetContext().GetSourceObject());

	VigorMagnitude = 80.0f + 2.5f * VigorMagnitude + 10.0f * AuraLevel; // calculation

	return VigorMagnitude;
}
