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
	
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,  Category = "EasySettings|ValueDiscreteDynamic_Number")
	void SetValue_Logic(int NewValue);
	UFUNCTION(BlueprintCallable, Category = "EasySettings|ValueDiscreteDynamic_Number")
	virtual void SetValue(int NewValue);
	
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "EasySettings|ValueDiscreteDynamic_Number")
	int GetValue() const;

	virtual FText GetCurrentOptionName() override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueChanged, int, NewValue);
	UPROPERTY(BlueprintAssignable, Category = "EasySettings|ValueDiscreteDynamic_Number")
	FOnValueChanged OnValueChanged;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Easy Settings")
	void NextOption();
	virtual void NextOption_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Easy Settings")
	void PreviousOption();
	virtual void PreviousOption_Implementation();
	
	
};
