// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EasySettingBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract, DefaultToInstanced, EditInlineNew)
class EASYSETTINGS_API UEasySettingBase : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingChanged, UEasySettingBase*, Setting);
	
	UPROPERTY(BlueprintAssignable, Category="Easy Settings")
	FOnSettingChanged OnSettingChanged;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Easy Settings")
	TArray<FText> Options;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Easy Settings")
	int32 CurrentOptionIndex = 0;
	
	UFUNCTION(BlueprintCallable, Category="Easy Settings")
	void AddOption(const FText& NewOption)
	{
		Options.Add(NewOption);
		OnSettingChanged.Broadcast(this);
	}

	UFUNCTION(BlueprintCallable, Category="Easy Settings")
	void RemoveOptionByIndex(const int32 Index)
	{
		if (Options.IsValidIndex(Index))
		{
			Options.RemoveAt(Index);
			OnSettingChanged.Broadcast(this);
		}
	}

	UFUNCTION(BlueprintCallable, Category="Easy Settings")
	virtual FText GetCurrentOptionName();
	UFUNCTION(BlueprintCallable, Category="Easy Settings")
	FText GetOptionNameByIndex(int32 Index) const
	{
		return Options.IsValidIndex(Index) ? Options[Index] : FText::FromString("Invalid Index");
	};
};
