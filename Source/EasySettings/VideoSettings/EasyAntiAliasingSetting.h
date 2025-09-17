#pragma once

#include "CoreMinimal.h"
#include "EasySettings/SettingsType/EasySettingValueDiscreteDynamic_Number.h"
#include "EasyAntiAliasingSetting.generated.h"

UCLASS()
class EASYSETTINGS_API UEasyAntiAliasingSetting : public UEasySettingValueDiscreteDynamic_Number
{
	GENERATED_BODY()

public:
	virtual void SetValue_Logic_Implementation(int NewValue) override;
	virtual int GetValue_Implementation() const override;

	UEasyAntiAliasingSetting();

	virtual bool IsDisplayConditionMet_Implementation() override;
};
