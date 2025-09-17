#pragma once

#include "CoreMinimal.h"
#include "EasySettings/SettingsType/EasySettingValueDiscreteDynamic_Number.h"
#include "EasyShadingSetting.generated.h"

UCLASS()
class EASYSETTINGS_API UEasyShadingSetting : public UEasySettingValueDiscreteDynamic_Number
{
	GENERATED_BODY()

public:
	virtual void SetValue_Logic_Implementation(int NewValue) override;
	virtual int GetValue_Implementation() const override;

	UEasyShadingSetting();
	virtual bool IsDisplayConditionMet_Implementation() override;
};
