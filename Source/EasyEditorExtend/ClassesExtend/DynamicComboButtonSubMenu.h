// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DynamicComboButtonExtend.h"
#include "DynamicComboButtonSubMenu.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,DefaultToInstanced,EditInlineNew)
class EASYEDITOREXTEND_API UDynamicComboButtonSubMenu : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<FComboButtonList> ButtonsList;
};
