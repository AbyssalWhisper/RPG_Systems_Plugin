// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyShadowSetting.h"

#include "GameFramework/GameUserSettings.h"

void UEasyShadowSetting::SetValue_Logic_Implementation(int NewValue)
{
	// set the shadow quality
	UGameUserSettings::GetGameUserSettings()->SetShadowQuality(NewValue);
	OnSettingChanged.Broadcast(this);
	OnValueChanged.Broadcast(NewValue);
	
}

int UEasyShadowSetting::GetValue_Implementation() const
{
	return UGameUserSettings::GetGameUserSettings()->GetShadowQuality();
}

UEasyShadowSetting::UEasyShadowSetting()
{
	SettingName = NSLOCTEXT("EasySettings", "ShadowSetting", "Shadow Quality");
}
