// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DynamicComboButtonExtend.generated.h"

/**
 * 
 */
class UEasyEditorObjectExecuteCode;

class UEasyComboButtonComponent;


UCLASS()
class EASYEDITOREXTEND_API UDynamicComboButtonExtend : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName ButtonName = "MyButtonNameId";
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText ButtonTooltip = FText::FromString("MyButton Tooltip");
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText ButtonLabel = FText::FromString("MyButton");
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bInSimpleComboBox=false;
	FName IconID = "SettingsEditor.Collision_Engine";
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Instanced,meta = (ShowOnlyInnerProperties,TitleProperty = ButtonLabel))
	TArray<UEasyComboButtonComponent*> ButtonsList;
};
