// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"

UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
	bHasStartupAbilitiesGiven = false;
}

void UAuraAbilitySystemComponent::PostInitAbilityActorInfo()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientOnGameplayEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(Ability_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	
	bHasStartupAbilitiesGiven = true;
	OnAbilityGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& Tag)
{
	if (!Tag.IsValid()) { return; }

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag) && !AbilitySpec.IsActive())
		{
			AbilitySpecInputPressed(AbilitySpec);
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& Tag)
{
	if (!Tag.IsValid()) { return; }

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& Tag)
{
	if (!Tag.IsValid()) { return; }

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UAuraAbilitySystemComponent::ExecuteDelegateForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (const FGameplayTag& Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(Ability_Default.GetTag().RequestDirectParent()))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(InputTag_Default.GetTag().RequestDirectParent()))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(Ability_Status_Default.GetTag().RequestDirectParent()))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability.Get()->AbilityTags.HasTagExact(AbilityTag))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		return GetAbilityStatusTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::SetAbilityStatusTagsByLevel(const int32& InLevel)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAuraAbilityInfo& Info : AbilityInfo->AbilityInfo)
	{
		if (Info.AbilityLevelRequirement > InLevel) { continue; }
		if (GetAbilitySpecFromAbilityTag(Info.AbilityTag) == nullptr) // Create and give ability if it hasn't already
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(Ability_Status_Unlocked);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientSetAbilityStatusTag(Info.AbilityTag, Ability_Status_Unlocked, 1);
		}
	}
}

void UAuraAbilitySystemComponent::SetAttributeValueByTag(const FGameplayTag& AttributeTag)
{
	ServerSetAttributeValueByTag(AttributeTag);
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag)
{
	FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag);
	if (!AbilitySpec) { return; }

	const FGameplayTag& OldInputTag = GetInputTagFromSpec(*AbilitySpec);
	const FGameplayTag& StatusTag = GetAbilityStatusTagFromSpec(*AbilitySpec);
	if (StatusTag.MatchesTagExact(Ability_Status_Equipped) || StatusTag.MatchesTagExact(Ability_Status_Acquired))
	{
		RemoveInputTag(InputTag);
		RemoveInputTagByAbility(AbilitySpec);
		AbilitySpec->DynamicAbilityTags.AddTag(InputTag);

		if (StatusTag.MatchesTagExact(Ability_Status_Acquired))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(Ability_Status_Acquired);
			AbilitySpec->DynamicAbilityTags.AddTag(Ability_Status_Equipped);
		}
		MarkAbilitySpecDirty(*AbilitySpec);
	}
	ClientEquipAbility(AbilityTag, Ability_Status_Equipped, InputTag, OldInputTag);
}

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutCurrentDesc, FString& OutNextLevelDesc)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutCurrentDesc = AuraAbility->GetDescriptionByLevel(AbilitySpec->Level);
			OutNextLevelDesc = AuraAbility->GetDescriptionByNextLevel(AbilitySpec->Level + 1);
			return true;
		}
	}

	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(Ability_None))
	{
		OutCurrentDesc = FString();
	}
	else
	{
		const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
		OutCurrentDesc = UAuraGameplayAbility::GetLockedDescription(AbilityInfo->GetAbilityInfoByTag(AbilityTag).AbilityLevelRequirement);
	}

	OutNextLevelDesc = FString();
	
	return false;
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetAbilitySpecFromAbilityTag(AbilityTag))
	{
		const FGameplayTag StatusTag = GetAbilityStatusTagFromSpec(*AbilitySpec);
		IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		if (StatusTag.MatchesTagExact(Ability_Status_Unlocked))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(Ability_Status_Unlocked);
			AbilitySpec->DynamicAbilityTags.AddTag(Ability_Status_Acquired);
			ClientSetAbilityStatusTag(AbilityTag, Ability_Status_Acquired, AbilitySpec->Level);
		}
		else if (StatusTag.MatchesTagExact(Ability_Status_Acquired) || StatusTag.MatchesTagExact(Ability_Status_Equipped))
		{
			++AbilitySpec->Level;
			ClientSetAbilityStatusTag(AbilityTag, StatusTag, AbilitySpec->Level);
		}
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ServerSetAttributeValueByTag_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.0f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);
	IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
}

void UAuraAbilitySystemComponent::ClientSetAbilityStatusTag_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel)
{
	OnAbilityStatusTagChangeDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& OldInputTag)
{
	OnAbilityEquippedDelegate.Broadcast(AbilityTag, StatusTag, InputTag, OldInputTag);
}

void UAuraAbilitySystemComponent::RemoveInputTagByAbility(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag& SlotInputTag = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->DynamicAbilityTags.RemoveTag(SlotInputTag);
	MarkAbilitySpecDirty(*AbilitySpec);
}

void UAuraAbilitySystemComponent::RemoveInputTag(const FGameplayTag& InputTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (HasAbilityInputTag(&AbilitySpec, InputTag))
		{
			RemoveInputTagByAbility(&AbilitySpec);
		}
	}
}

bool UAuraAbilitySystemComponent::HasAbilityInputTag(const FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& InputTag)
{
	for (const FGameplayTag& Tag : AbilitySpec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(InputTag))
		{
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bHasStartupAbilitiesGiven)
	{
		OnAbilityGivenDelegate.Broadcast();
		bHasStartupAbilitiesGiven = true;
	}
}

void UAuraAbilitySystemComponent::ClientOnGameplayEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle GEHandle) const
{
	FGameplayTagContainer TagContainer;
	GESpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
