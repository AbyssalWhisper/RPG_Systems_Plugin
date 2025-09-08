// Fill out your copyright notice in the Description page of Project Settings.


#include "EasySettingsSubsystem.h"

#include "EasySettingBase.h"

void UEasySettingsSubsystem::InitializeSettings(TArray<TSubclassOf<class UEasySettingBase>> SettingsClasses)
{
	for (TSubclassOf<UEasySettingBase> SettingClass : SettingsClasses)
	{
		if (SettingClass)
		{
			UEasySettingBase* NewSetting = NewObject<UEasySettingBase>(this, SettingClass);
			if (NewSetting)
			{
				Settings.Add(NewSetting);
				UE_LOG(LogTemp, Warning, TEXT("Added Setting: %s"), *NewSetting->GetClass()->GetName());
			}
		}
	}
	InitializeDependencies();
}

UEasySettingBase* UEasySettingsSubsystem::GetSettingByClass(TSubclassOf<class UEasySettingBase> SettingClass) const
{
	for (UEasySettingBase* Setting : Settings)
	{
		UE_LOG(LogTemp, Warning, TEXT("Have Setting: %s"), *Setting->GetClass()->GetName());
		if (Setting->GetClass() == SettingClass)
		{
			return Setting;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No match for: %s"), *SettingClass->GetName());
	return nullptr;
}

void UEasySettingsSubsystem::InitializeDependencies()
{
	for (UEasySettingBase* Setting : Settings)
	{
		for (auto DependencyClass : Setting->SettingsDependency)
		{
			if (DependencyClass)
			{
				UEasySettingBase* DependencySetting = GetSettingByClass(DependencyClass);
				if (DependencySetting)
				{
					DependencySetting->OnSettingChanged.AddDynamic(Setting, &UEasySettingBase::OnDependentConfigurationChanged_Internal);
					
				}
				UE_LOG(LogTemp, Warning, TEXT("Setting %s depends on %s"), *Setting->GetClass()->GetName(), *DependencySetting->GetClass()->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Dependency %s for setting %s not found"), *DependencyClass->GetName(), *Setting->GetClass()->GetName());
			}
		}
	}
}
