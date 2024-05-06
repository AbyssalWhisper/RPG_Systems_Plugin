// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DynamicComboButtonExtend.generated.h"

/**
 * 
 */
class UEasyEditorObjectExecuteCode;

class UDynamicComboButtonSubMenu;





USTRUCT(BlueprintType)
struct FComboButtonList
{
	GENERATED_BODY()
public:

	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText ButtonTooltip = FText();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText ButtonLabel = FText();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bIsSubMenu = false;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Instanced,meta = (ShowOnlyInnerProperties,EditCondition="bIsSubMenu==false",EditConditionHides))
	UEasyEditorObjectExecuteCode* CustomExecuteCode;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Instanced,meta = (ShowOnlyInnerProperties,EditCondition="bIsSubMenu==true",EditConditionHides))
	UDynamicComboButtonSubMenu* SubMenuButtonsList;
};

UCLASS()
class EASYEDITOREXTEND_API UDynamicComboButtonExtend : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName ButtonName = "";
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText ButtonTooltip = FText();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText ButtonLabel = FText();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bInSimpleComboBox=false;
	FName IconID = "SettingsEditor.Collision_Engine";
	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta = (ShowOnlyInnerProperties))
	TArray<FComboButtonList> ButtonsList;
};
