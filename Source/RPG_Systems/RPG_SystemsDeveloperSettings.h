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
	UPROPERTY(EditAnywhere, config,NoClear, Category = "General Settings")
	TSet<TSoftObjectPtr<UInputAction>> AbilitiesInputActions;
	
	
};
