// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	ActorLevel = 1.0f;
}

void AAuraEffectActor::ApplyEffectToActor(AActor* Actor, const TSubclassOf<UGameplayEffect> GameplayEffectClass) const
{
	UAbilitySystemComponent* ActorASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!ActorASC) { return; }
	check(GameplayEffectClass);

	FGameplayEffectContextHandle GEContextHandle = ActorASC->MakeEffectContext();
	GEContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GESpecHandle = ActorASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, GEContextHandle);
	ActorASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data.Get());
}

void AAuraEffectActor::RemoveEffectFromActor(AActor* Actor, const TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* ActorASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!ActorASC) { return; }
	check(GameplayEffectClass);

	ActorASC->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass, ActorASC, 1);
}

void AAuraEffectActor::OnBeginOverlap(AActor* OtherActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToActor(OtherActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToActor(OtherActor, DurationGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* OtherActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToActor(OtherActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToActor(OtherActor, DurationGameplayEffectClass);
	}
}
