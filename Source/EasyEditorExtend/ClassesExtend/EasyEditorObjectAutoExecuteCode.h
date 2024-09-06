// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EasyEditorExtend/ClassesExtend/EasyEditorObjectExecuteCode.h"
#include "EasyEditorObjectAutoExecuteCode.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class EASYEDITOREXTEND_API UEasyEditorObjectAutoExecuteCode : public UEasyEditorObjectExecuteCode
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void Execute();
};
