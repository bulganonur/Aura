// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/AuraTask_TargetDataUnderCursor.h"
#include "AbilitySystemComponent.h"

UAuraTask_TargetDataUnderCursor* UAuraTask_TargetDataUnderCursor::CreateTargetDataUnderCursorProxy(UGameplayAbility* OwningAbility)
{
	UAuraTask_TargetDataUnderCursor* MyObj = NewAbilityTask<UAuraTask_TargetDataUnderCursor>(OwningAbility);

	return MyObj;
}

void UAuraTask_TargetDataUnderCursor::Activate()
{
	bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	// if client, then ServerSetReplicatedTargetData. this sends local/client TargetData to server
	if (bIsLocallyControlled)
	{
		SendUnderCursorData();
	}
	else // if server, TargetData is already set so bind a callback function which broadcasts the TargetData [this is *probably* for the listen server]. The ability owns this task is listening.
	{
		const FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();
		const FPredictionKey OriginalPredictionKey = GetActivationPredictionKey();
		
		AbilitySystemComponent->AbilityTargetDataSetDelegate(AbilitySpecHandle, OriginalPredictionKey).AddUObject(this, &ThisClass::OnTargetDataReplicatedCallback);

		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(AbilitySpecHandle, OriginalPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UAuraTask_TargetDataUnderCursor::SendUnderCursorData()
{
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	if (APawn* Avatar = Cast<APawn>(GetAvatarActor()))
	{
		if (const APlayerController* AvatarController = Cast<APlayerController>(Avatar->GetController()))
		{
			FHitResult CursorHit;
			if (AvatarController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit))
			{
				// Store CursorHit/HitResult in a TargetData struct
				FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit;
				TargetData->HitResult = CursorHit;

				// Pack it in a handle
				FGameplayAbilityTargetDataHandle TargetDataHandle;
				TargetDataHandle.Add(TargetData);

				// Send the handle to server
				AbilitySystemComponent->ServerSetReplicatedTargetData
				(
					GetAbilitySpecHandle(),
					GetActivationPredictionKey(),
					TargetDataHandle,
					FGameplayTag(),
					AbilitySystemComponent->ScopedPredictionKey
				);

				// Is ability still active, if so Broadcast TargetData. The ability owns this task is listening.
				if (ShouldBroadcastAbilityTaskDelegates())
				{
					ValidData.Broadcast(TargetDataHandle);
				}
			}
		}
	}
}

void UAuraTask_TargetDataUnderCursor::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	// Is ability still active, if so Broadcast TargetData. The ability owns this task is listening.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
