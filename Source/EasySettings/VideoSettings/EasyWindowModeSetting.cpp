// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyWindowModeSetting.h"

#include "GameFramework/GameUserSettings.h"

void UEasyWindowModeSetting::SetValue_Logic_Implementation(int NewValue)
{
	// Convert int to window mode enum
	EWindowMode::Type WindowMode = static_cast<EWindowMode::Type>(NewValue);
	
	// Set the window mode
	UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(WindowMode);
	OnSettingChanged.Broadcast(this);
	OnValueChanged.Broadcast(NewValue);
}

int UEasyWindowModeSetting::GetValue_Implementation() const
{
	return static_cast<int>(UGameUserSettings::GetGameUserSettings()->GetFullscreenMode());
}

UEasyWindowModeSetting::UEasyWindowModeSetting()
{
	SettingName = NSLOCTEXT("EasySettings", "WindowModeSetting", "Window Mode");
	
	// Clear default options and add window mode specific options
	Options.Empty();
	Options.Add(NSLOCTEXT("EasySettings", "WindowMode_Fullscreen", "Fullscreen"));
	Options.Add(NSLOCTEXT("EasySettings", "WindowMode_WindowedFullscreen", "Windowed Fullscreen"));
	Options.Add(NSLOCTEXT("EasySettings", "WindowMode_Windowed", "Windowed"));
}

bool UEasyWindowModeSetting::IsDisplayConditionMet_Implementation()
{
	return Super::IsDisplayConditionMet_Implementation();
}
