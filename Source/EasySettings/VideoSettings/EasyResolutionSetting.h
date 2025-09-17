// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasySettings/SettingsType/EasySettingValueDiscreteDynamic_Number.h"
#include "EasyResolutionSetting.generated.h"

/**
 * 
 */
UCLASS()
class EASYSETTINGS_API UEasyResolutionSetting : public UEasySettingValueDiscreteDynamic_Number
{
	GENERATED_BODY()

public:
	virtual void SetValue_Logic_Implementation(int NewValue) override;
	virtual int GetValue_Implementation() const override;
	
	UEasyResolutionSetting();

	virtual bool IsDisplayConditionMet_Implementation() override;

private:
	UPROPERTY()
	TArray<FIntPoint> AvailableResolutions;
	virtual void Init_Implementation() override;

public:
	virtual void OnDependentConfigurationChanged_Internal(UEasySettingBase* Setting) override;
	void RefreshResolutions();
};
