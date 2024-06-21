// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/PrimaryAssetLabel.h"
#include "EasyEditorAutoExecuteCode.generated.h"

/**
 * 
 */
class UEasyEditorObjectAutoExecuteCode;
UCLASS()
class EASYEDITOREXTEND_API UEasyEditorAutoExecuteCode : public UPrimaryDataAsset
{

	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Instanced, meta = (ShowOnlyInnerProperties))
	UEasyEditorObjectAutoExecuteCode* CustomExecuteCode;
};
