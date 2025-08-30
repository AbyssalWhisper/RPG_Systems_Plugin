// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasySettings/EasySettingBase.h"
#include "EasySettingValueDiscreteDynamic_Number.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class EASYSETTINGS_API UEasySettingValueDiscreteDynamic_Number : public UEasySettingBase
{
	GENERATED_BODY()
public:
	UEasySettingValueDiscreteDynamic_Number();
	
	UFUNCTION(BlueprintCallable, Category = "EasySettings|ValueDiscreteDynamic_Number")
	virtual void SetValue(int NewValue);
	UFUNCTION(BlueprintCallable, Category = "EasySettings|ValueDiscreteDynamic_Number")
	virtual int GetValue() const;

	virtual FText GetCurrentOptionName() override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueChanged, int, NewValue);
	UPROPERTY(BlueprintAssignable, Category = "EasySettings|ValueDiscreteDynamic_Number")
	FOnValueChanged OnValueChanged;
};
