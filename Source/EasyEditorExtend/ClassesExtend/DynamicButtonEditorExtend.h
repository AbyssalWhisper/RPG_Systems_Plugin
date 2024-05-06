// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DynamicButtonEditorExtend.generated.h"

/**
 * 
 */
class UEasyEditorObjectExecuteCode;
UCLASS()
class EASYEDITOREXTEND_API UDynamicButtonEditorExtend : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName ButtonName = "";
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText ButtonTooltip = FText();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText ButtonLabel = FText();
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Instanced,meta = (ShowOnlyInnerProperties))
	UEasyEditorObjectExecuteCode* CustomExecuteCode;
	UFUNCTION()
	void Execute();
};
