// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE(FOnAbilityGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAbilityStatusTagChangeSignature, const FGameplayTag& /** AbilityTag */, const FGameplayTag& /** StatusTag */, const int32 /** AbilityLevel */);
DECLARE_MULTICAST_DELEGATE_FourParams(FOnAbilityEquipped, const FGameplayTag& /** AbilityTag */, const FGameplayTag& /** StatusTag */, const FGameplayTag& /** InputTag */, const FGameplayTag& /** OldInputTag */);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	
	UAuraAbilitySystemComponent();
	
	FEffectAssetTags EffectAssetTags;
	FOnAbilityGiven OnAbilityGivenDelegate;
	FOnAbilityStatusTagChangeSignature OnAbilityStatusTagChangeDelegate;
	FOnAbilityEquipped OnAbilityEquippedDelegate;
	void ExecuteDelegateForEachAbility(const FForEachAbility& Delegate);

	bool bHasStartupAbilitiesGiven;

	void PostInitAbilityActorInfo();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void AbilityInputTagHeld(const FGameplayTag& Tag);
	void AbilityInputTagReleased(const FGameplayTag& Tag);
	void AbilityInputTagPressed(const FGameplayTag& Tag);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetAbilityStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static bool HasAbilityInputTag(const FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& InputTag);

	FGameplayAbilitySpec* GetAbilitySpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutCurrentDesc, FString& OutNextLevelDesc);
	void SetAbilityStatusTagsByLevel(const int32& InLevel);
	
	void SetAttributeValueByTag(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerSetAttributeValueByTag(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& InputTag);

	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& InputTag, const FGameplayTag& OldInputTag);

	void RemoveInputTagByAbility(FGameplayAbilitySpec* AbilitySpec);
	void RemoveInputTag(const FGameplayTag& InputTag);
	
protected:

	UFUNCTION(Client, Reliable)
	void ClientOnGameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle GEHandle) const;

	UFUNCTION(Client, Reliable)
	void ClientSetAbilityStatusTag(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel);
	
	virtual void OnRep_ActivateAbilities() override;
};
