// Fill out your copyright notice in the Description page of Project Settings.

#include "EasyPostProcessingSetting.h"
#include "GameFramework/GameUserSettings.h"

void UEasyPostProcessingSetting::SetValue_Logic_Implementation(int NewValue)
{
	UGameUserSettings::GetGameUserSettings()->SetPostProcessingQuality(NewValue);
	OnSettingChanged.Broadcast(this);
	OnValueChanged.Broadcast(NewValue);
}

int UEasyPostProcessingSetting::GetValue_Implementation() const
{
	return UGameUserSettings::GetGameUserSettings()->GetPostProcessingQuality();
}

UEasyPostProcessingSetting::UEasyPostProcessingSetting()
{
    SettingName = NSLOCTEXT("EasySettings", "PostProcessingSetting", "Post Processing Quality");
}

bool UEasyPostProcessingSetting::IsDisplayConditionMet_Implementation()
{
	return Super::IsDisplayConditionMet_Implementation();
}
