// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyShadowSetting.h"

#include "GameFramework/GameUserSettings.h"

void UEasyShadowSetting::SetValue(int NewValue)
{
	// set the shadow quality
	UGameUserSettings::GetGameUserSettings()->SetShadowQuality(NewValue);
	OnSettingChanged.Broadcast(this);
	OnValueChanged.Broadcast(NewValue);
}

int UEasyShadowSetting::GetValue() const
{
	return UGameUserSettings::GetGameUserSettings()->GetShadowQuality();
}

UEasyShadowSetting::UEasyShadowSetting()
{
	// add options with localized text
	
}


