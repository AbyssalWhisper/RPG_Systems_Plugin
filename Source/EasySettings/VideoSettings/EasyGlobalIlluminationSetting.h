#pragma once

#include "CoreMinimal.h"
#include "EasySettings/SettingsType/EasySettingValueDiscreteDynamic_Number.h"
#include "EasyGlobalIlluminationSetting.generated.h"

UCLASS()
class EASYSETTINGS_API UEasyGlobalIlluminationSetting : public UEasySettingValueDiscreteDynamic_Number
{
	GENERATED_BODY()

public:
	virtual void SetValue_Logic_Implementation(int NewValue) override;
	virtual int GetValue_Implementation() const override;

	UEasyGlobalIlluminationSetting();
};
