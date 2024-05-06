// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "DynamicButtonEditorExtendExecuteCode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType,DefaultToInstanced,EditInlineNew)
class EASYEDITOREXTEND_API UDynamicButtonEditorExtendExecuteCode : public UObject
{
	GENERATED_BODY()
public:


	UFUNCTION(BlueprintNativeEvent)
	void Execute();
};
