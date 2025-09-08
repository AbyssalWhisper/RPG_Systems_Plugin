// Fill out your copyright notice in the Description page of Project Settings.

#include "EasyAntiAliasingSetting.h"
#include "GameFramework/GameUserSettings.h"

void UEasyAntiAliasingSetting::SetValue_Logic_Implementation(int NewValue)
{
	UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(NewValue);
	OnSettingChanged.Broadcast(this);
	OnValueChanged.Broadcast(NewValue);
}

int UEasyAntiAliasingSetting::GetValue_Implementation() const
{
	return UGameUserSettings::GetGameUserSettings()->GetAntiAliasingQuality();
}

UEasyAntiAliasingSetting::UEasyAntiAliasingSetting()
{
	SettingName = NSLOCTEXT("EasySettings", "AntiAliasingSetting", "Anti-Aliasing Quality");
}
