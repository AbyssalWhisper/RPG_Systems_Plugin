// Fill out your copyright notice in the Description page of Project Settings.

#include "EasyResolutionSetting.h"

#include "EasyWindowModeSetting.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/Engine.h"
#include "RHI.h"
#include "EasySettings/EasySettingsSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UEasyResolutionSetting::SetValue_Logic_Implementation(int NewValue)
{
	if (AvailableResolutions.IsValidIndex(NewValue))
	{
		FIntPoint SelectedResolution = AvailableResolutions[NewValue];
		
		// Set the screen resolution
		UGameUserSettings::GetGameUserSettings()->SetScreenResolution(SelectedResolution);
		OnSettingChanged.Broadcast(this);
		OnValueChanged.Broadcast(NewValue);
	}
}

int UEasyResolutionSetting::GetValue_Implementation() const
{
	FIntPoint CurrentResolution = UGameUserSettings::GetGameUserSettings()->GetScreenResolution();
	
	// Find the index of current resolution in available resolutions
	for (int32 i = 0; i < AvailableResolutions.Num(); i++)
	{
		if (AvailableResolutions[i] == CurrentResolution)
		{
			return i;
		}
	}
	
	// If current resolution not found, return 0 (first resolution)
	return 0;
}

UEasyResolutionSetting::UEasyResolutionSetting()
{
	SettingName = NSLOCTEXT("EasySettings", "ResolutionSetting", "Screen Resolution");
	SettingsDependency.Add(UEasyWindowModeSetting::StaticClass());
}

bool UEasyResolutionSetting::IsDisplayConditionMet_Implementation()
{
	//get EasySettings subsystem
	return Super::IsDisplayConditionMet_Implementation();
}

void UEasyResolutionSetting::Init_Implementation()
{
	RefreshResolutions();
}

void UEasyResolutionSetting::OnDependentConfigurationChanged_Internal(UEasySettingBase* Setting)
{
	RefreshResolutions();
	Super::OnDependentConfigurationChanged_Internal(Setting);
	
}

void UEasyResolutionSetting::RefreshResolutions()
{
	// Clear default options
	Options.Empty();
	AvailableResolutions.Empty();

	switch (UGameUserSettings::GetGameUserSettings()->GetFullscreenMode())
	{
	case EWindowMode::Fullscreen:
		UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);
		break;
	case EWindowMode::WindowedFullscreen:
		UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);
		
		break;
	case EWindowMode::Windowed:
		UKismetSystemLibrary::GetConvenientWindowedResolutions(AvailableResolutions);
		break;
	case EWindowMode::NumWindowModes:
		UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);
		
		break;
	}
		

	// Add supported resolutions to our arrays
	for (auto Resolution : AvailableResolutions)
	{
		AvailableResolutions.Add(Resolution);
				
		
				
		Options.Add(FText::FromString(Resolution.ToString()));
	}
	
	// Fallback: If no resolutions found, add common ones
	if (AvailableResolutions.Num() == 0)
	{
		TArray<FIntPoint> FallbackResolutions = {
			FIntPoint(1920, 1080), // 1080p
			FIntPoint(1600, 900),  // HD+
			FIntPoint(1366, 768),  // WXGA
			FIntPoint(1280, 720),  // HD
			FIntPoint(1024, 768)   // XGA
		};
		
		for (const FIntPoint& Resolution : FallbackResolutions)
		{
			AvailableResolutions.Add(Resolution);
			
			FText ResolutionText = FText::Format(
				NSLOCTEXT("EasySettings", "Resolution_Format", "{0} x {1}"),
				FText::AsNumber(Resolution.X),
				FText::AsNumber(Resolution.Y)
			);
			
			Options.Add(ResolutionText);
		}
	}
}
