// Fill out your copyright notice in the Description page of Project Settings.


#include "EasySettingValueDiscreteDynamic_Number.h"

UEasySettingValueDiscreteDynamic_Number::UEasySettingValueDiscreteDynamic_Number()
{
	Options.Add(NSLOCTEXT("EasySettings", "Quality_Low", "Low"));
	Options.Add(NSLOCTEXT("EasySettings", "Quality_Medium", "Medium"));
	Options.Add(NSLOCTEXT("EasySettings", "Quality_High", "High"));
	Options.Add(NSLOCTEXT("EasySettings", "Quality_Epic", "Epic"));
	Options.Add(NSLOCTEXT("EasySettings", "Quality_Cinematic", "Cinematic"));
}

void UEasySettingValueDiscreteDynamic_Number::SetValue(int NewValue)
{
}

int UEasySettingValueDiscreteDynamic_Number::GetValue() const
{
	return 0;
}

FText UEasySettingValueDiscreteDynamic_Number::GetCurrentOptionName()
{
	return GetOptionNameByIndex(GetValue());
}
