// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"

#include "EasySettingsConfig.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSettingsSection : public FTableRowBase
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<TSubclassOf<class UEasySettingBase>> Settings;
};

UCLASS(config = Game, DefaultConfig, meta = (DisplayName = "EasySettings"))
class EASYSETTINGS_API UEasySettingsConfig : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Config, Category = "Settings")
	TMap<FName, FSettingsSection> SettingsSections;

	UFUNCTION(BlueprintCallable ,BlueprintPure,Category = "RPG Systems Settings")
	static UEasySettingsConfig* GetEasySettings()
	{
		return GetMutableDefault<UEasySettingsConfig>();
	}
};
