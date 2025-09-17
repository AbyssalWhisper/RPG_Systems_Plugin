// Fill out your copyright notice in the Description page of Project Settings.


#include "EasySettingValueDiscreteDynamic_Float.h"

UEasySettingValueDiscreteDynamic_Float::UEasySettingValueDiscreteDynamic_Float()
{

}

void UEasySettingValueDiscreteDynamic_Float::SetValue_Logic_Implementation(float NewValue)
{
	// Implementation to be overridden in Blueprint or derived classes
}

float UEasySettingValueDiscreteDynamic_Float::GetValue_Implementation() const
{
	// Default implementation - should be overridden
	return FMath::Clamp(1.0f, MinValue, MaxValue);
}

void UEasySettingValueDiscreteDynamic_Float::SetValue(float NewValue)
{
	float ClampedValue = FMath::Clamp(NewValue, MinValue, MaxValue);
	SetValue_Logic(ClampedValue);
	OnValueChanged.Broadcast(ClampedValue);
	OnSettingChanged.Broadcast(this);
}

FText UEasySettingValueDiscreteDynamic_Float::GetCurrentOptionName()
{
	return GetOptionNameByIndex(GetValue());
}

