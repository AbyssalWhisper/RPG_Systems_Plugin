// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputDeveloperSettings.h"
#include "Engine/DeveloperSettings.h"
#include "RPG_SystemsDeveloperSettings.generated.h"

/**
 * 
 */
class UInputAction;
UCLASS(config = Game, DefaultConfig, meta = (DisplayName = "RPG Systems Settings"))

class RPG_SYSTEMS_API URPG_SystemsDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, config,NoClear, Category = "General Settings")
	TSet<TSoftObjectPtr<UInputAction>> AbilitiesInputActions;
	
	UFUNCTION(BlueprintCallable ,BlueprintPure,Category = "RPG Systems Settings")
	static URPG_SystemsDeveloperSettings* GetRPGSystemsSettings()
	{
		return GetMutableDefault<URPG_SystemsDeveloperSettings>();
	}
};
