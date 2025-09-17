// Fill out your copyright notice in the Description page of Project Settings.


#include "EasySettingBase.h"


FText UEasySettingBase::GetCurrentOptionName()
{
	return FText::GetEmpty();
}

void UEasySettingBase::Init_Implementation()
{
}

bool UEasySettingBase::IsDisplayConditionMet_Implementation()
{
	return true;
}

bool UEasySettingBase::EditConditionMet_Implementation()
{
	return true;
}


