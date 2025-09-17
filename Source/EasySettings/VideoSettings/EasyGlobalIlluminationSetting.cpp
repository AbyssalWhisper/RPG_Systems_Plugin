// Fill out your copyright notice in the Description page of Project Settings.

#include "EasyGlobalIlluminationSetting.h"
#include "GameFramework/GameUserSettings.h"

void UEasyGlobalIlluminationSetting::SetValue_Logic_Implementation(int NewValue)
{
	UGameUserSettings::GetGameUserSettings()->SetGlobalIlluminationQuality(NewValue);
	OnSettingChanged.Broadcast(this);
	OnValueChanged.Broadcast(NewValue);
}

int UEasyGlobalIlluminationSetting::GetValue_Implementation() const
{
	return UGameUserSettings::GetGameUserSettings()->GetGlobalIlluminationQuality();
}

UEasyGlobalIlluminationSetting::UEasyGlobalIlluminationSetting()
{
	SettingName = NSLOCTEXT("EasySettings", "GlobalIlluminationSetting", "Global Illumination Quality");
}

bool UEasyGlobalIlluminationSetting::IsDisplayConditionMet_Implementation()
{
	return Super::IsDisplayConditionMet_Implementation();
}
