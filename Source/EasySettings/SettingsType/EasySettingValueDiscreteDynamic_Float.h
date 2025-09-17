// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasySettings/EasySettingBase.h"
#include "EasySettingValueDiscreteDynamic_Float.generated.h"

/**
 * Setting class for discrete dynamic float values, ideal for scales like resolution scaling or volume controls
 */
UCLASS(Abstract, Blueprintable)
class EASYSETTINGS_API UEasySettingValueDiscreteDynamic_Float : public UEasySettingBase
{
	GENERATED_BODY()
public:
	UEasySettingValueDiscreteDynamic_Float();
	
	// Min and Max value properties
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EasySettings|ValueDiscreteDynamic_Float")
	float MinValue = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EasySettings|ValueDiscreteDynamic_Float")
	float MaxValue = 9999.f;
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,  Category = "EasySettings|ValueDiscreteDynamic_Float")
	void SetValue_Logic(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "EasySettings|ValueDiscreteDynamic_Float")
	virtual void SetValue(float NewValue);
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "EasySettings|ValueDiscreteDynamic_Float")
	float GetValue() const;

	virtual FText GetCurrentOptionName() override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueChanged, float, NewValue);
	UPROPERTY(BlueprintAssignable, Category = "EasySettings|ValueDiscreteDynamic_Float")
	FOnValueChanged OnValueChanged;
};
