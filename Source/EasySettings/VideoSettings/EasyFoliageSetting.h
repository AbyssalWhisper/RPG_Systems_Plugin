// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasySettings/SettingsType/EasySettingValueDiscreteDynamic_Number.h"
#include "EasyFoliageSetting.generated.h"

/**
 * Foliage quality setting wrapper
 */
UCLASS()
class EASYSETTINGS_API UEasyFoliageSetting : public UEasySettingValueDiscreteDynamic_Number
{
	GENERATED_BODY()

public:
	virtual void SetValue_Logic_Implementation(int NewValue) override;
	virtual int GetValue_Implementation() const override;
	
	UEasyFoliageSetting();

	virtual bool IsDisplayConditionMet_Implementation() override;
};
