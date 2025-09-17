// Fill out your copyright notice in the Description page of Project Settings.

#include "EasyReflectionsSetting.h"
#include "GameFramework/GameUserSettings.h"

void UEasyReflectionsSetting::SetValue_Logic_Implementation(int NewValue)
{
	UGameUserSettings::GetGameUserSettings()->SetReflectionQuality(NewValue);
	OnSettingChanged.Broadcast(this);
	OnValueChanged.Broadcast(NewValue);
}

int UEasyReflectionsSetting::GetValue_Implementation() const
{
	return UGameUserSettings::GetGameUserSettings()->GetReflectionQuality();
}

UEasyReflectionsSetting::UEasyReflectionsSetting()
{
	SettingName = NSLOCTEXT("EasySettings", "ReflectionsSetting", "Reflections Quality");
}

bool UEasyReflectionsSetting::IsDisplayConditionMet_Implementation()
{
	return Super::IsDisplayConditionMet_Implementation();
}
