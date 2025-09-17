// Fill out your copyright notice in the Description page of Project Settings.

#include "EasyEffectsSetting.h"
#include "GameFramework/GameUserSettings.h"

void UEasyEffectsSetting::SetValue_Logic_Implementation(int NewValue)
{
	UGameUserSettings::GetGameUserSettings()->SetVisualEffectQuality(NewValue);
	OnSettingChanged.Broadcast(this);
	OnValueChanged.Broadcast(NewValue);
}

int UEasyEffectsSetting::GetValue_Implementation() const
{
	return UGameUserSettings::GetGameUserSettings()->GetVisualEffectQuality();
}

UEasyEffectsSetting::UEasyEffectsSetting()
{
	SettingName = NSLOCTEXT("EasySettings", "EffectsSetting", "Effects Quality");
}

bool UEasyEffectsSetting::IsDisplayConditionMet_Implementation()
{
	return Super::IsDisplayConditionMet_Implementation();
}
