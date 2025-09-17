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

void UEasySettingValueDiscreteDynamic_Number::SetValue_Logic_Implementation(int NewValue)
{
}

int UEasySettingValueDiscreteDynamic_Number::GetValue_Implementation() const
{
	return 0;
}



void UEasySettingValueDiscreteDynamic_Number::SetValue(int NewValue)
{
	SetValue_Logic(NewValue);
	OnValueChanged.Broadcast(NewValue);
	OnSettingChanged.Broadcast(this);
}



FText UEasySettingValueDiscreteDynamic_Number::GetCurrentOptionName()
{
	return GetOptionNameByIndex(GetValue());
}

void UEasySettingValueDiscreteDynamic_Number::NextOption_Implementation()
{
	if (GetValue() + 1 < Options.Num())
	{
		SetValue(GetValue() + 1);
	}else
	{
		SetValue(0);
	}
}

void UEasySettingValueDiscreteDynamic_Number::PreviousOption_Implementation()
{
	if (GetValue() - 1 >= 0)
	{
		SetValue(GetValue() - 1);
	}
	else
	{
		SetValue(Options.Num() - 1);
	}
}
