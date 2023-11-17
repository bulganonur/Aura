// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	ActorLevel = 1.0f;
	bApplyEffectToEnemies = false;
}

void AAuraEffectActor::ApplyEffectToActor(AActor* Actor, const TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// @todo: find a better way to check for an enemy (perhaps Enemy Interface or GameplayTags?)
	if (Actor->ActorHasTag("Enemy") && !bApplyEffectToEnemies) { return ; }
	
	UAbilitySystemComponent* ActorASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!ActorASC) { return; }
	check(GameplayEffectClass);

	FGameplayEffectContextHandle GEContextHandle = ActorASC->MakeEffectContext();
	GEContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GESpecHandle = ActorASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, GEContextHandle);
	ActorASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data.Get());

	if (GESpecHandle.Data->Def.Get()->DurationPolicy != EGameplayEffectDurationType::Infinite)
	{
		Destroy();
	}
}

void AAuraEffectActor::RemoveEffectFromActor(AActor* Actor, const TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// @todo: find a better way to check for an enemy (perhaps Enemy Interface or GameplayTags?)
	if (Actor->ActorHasTag("Enemy") && !bApplyEffectToEnemies) { return ; }
	
	UAbilitySystemComponent* ActorASC =  UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!ActorASC) { return; }
	check(GameplayEffectClass);

	ActorASC->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass, ActorASC, 1);
}

/*void AAuraEffectActor::OnBeginOverlap(AActor* OtherActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToActor(OtherActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToActor(OtherActor, DurationGameplayEffectClass);
	}
}*/

/*void AAuraEffectActor::OnEndOverlap(AActor* OtherActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToActor(OtherActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToActor(OtherActor, DurationGameplayEffectClass);
	}
}*/
