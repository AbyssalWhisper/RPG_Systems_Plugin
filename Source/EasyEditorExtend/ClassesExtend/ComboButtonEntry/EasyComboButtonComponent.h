// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "EasyComboButtonComponent.generated.h"

/**
 * 
 */
class FMenuBuilder;
UCLASS(Abstract,BlueprintType,DefaultToInstanced,EditInlineNew,CollapseCategories)
class EASYEDITOREXTEND_API UEasyComboButtonComponent : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(DisplayPriority=0))
	FText ButtonLabel = FText::FromString("MyButton");
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(DisplayPriority=0))
	FText ButtonTooltip = FText::FromString("MyButton Tooltip");
	virtual void Execute(FMenuBuilder& MenuBuilder);
	
};
