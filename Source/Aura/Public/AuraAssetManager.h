// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * Aura Asset Manager
 *
 * Don't forget to add this class to DefaultEngine.ini file
 * Under [/Script/Engine.Engine] add this line AssetManagerClassName=/Script/Aura.AuraAssetManager
 * 
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UAuraAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
