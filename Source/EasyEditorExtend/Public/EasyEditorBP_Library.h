// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EasyEditorBP_Library.generated.h"

/**
 * 
 */
UCLASS()
class EASYEDITOREXTEND_API UEasyEditorBP_Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static UWorld* GetCurrentEditorWorld();
};
