// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasySettings/SettingsType/EasySettingValueDiscreteDynamic_Number.h"
#include "EasyWindowModeSetting.generated.h"

/**
 * 
 */
UCLASS()
class EASYSETTINGS_API UEasyWindowModeSetting : public UEasySettingValueDiscreteDynamic_Number
{
	GENERATED_BODY()

public:
	virtual void SetValue_Logic_Implementation(int NewValue) override;
	virtual int GetValue_Implementation() const override;
	
	UEasyWindowModeSetting();

	virtual bool IsDisplayConditionMet_Implementation() override;
};
