// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasySettings/SettingsType/EasySettingValueDiscreteDynamic_Number.h"
#include "EasyShadowSetting.generated.h"

/**
 * 
 */
UCLASS()
class EASYSETTINGS_API UEasyShadowSetting : public UEasySettingValueDiscreteDynamic_Number
{
	GENERATED_BODY()

public:
	virtual void SetValue(int NewValue) override;
	virtual int GetValue() const override;
	
	UEasyShadowSetting();
};
