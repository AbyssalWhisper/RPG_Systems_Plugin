// Fill out your copyright notice in the Description page of Project Settings.

#include "EasyShadingSetting.h"
#include "GameFramework/GameUserSettings.h"

void UEasyShadingSetting::SetValue_Logic_Implementation(int NewValue)
{
	UGameUserSettings::GetGameUserSettings()->SetShadingQuality(NewValue);
	OnSettingChanged.Broadcast(this);
	OnValueChanged.Broadcast(NewValue);
}

int UEasyShadingSetting::GetValue_Implementation() const
{
	return UGameUserSettings::GetGameUserSettings()->GetShadingQuality();
}

UEasyShadingSetting::UEasyShadingSetting()
{
	SettingName = NSLOCTEXT("EasySettings", "ShadingSetting", "Shading Quality");
}

bool UEasyShadingSetting::IsDisplayConditionMet_Implementation()
{
	return Super::IsDisplayConditionMet_Implementation();
}
