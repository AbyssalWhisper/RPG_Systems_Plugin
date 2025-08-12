// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetRegistry/AssetRegistryModule.h"
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
	UFUNCTION(BlueprintCallable, Category = "DynamicComboButtonExtend")
	static void CreateDymamicButtons();
	static void CreateComboButton(UDynamicComboButtonExtend* ButtonObject);

	UFUNCTION(BlueprintCallable, Category = "DynamicComboButtonExtend")
	static TArray<UDynamicComboButtonExtend*> GetAllDynamicComboButtonExtend()
	{
		TArray<UDynamicComboButtonExtend*> Items;
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		TArray<FAssetData> AssetData;
		FTopLevelAssetPath ClassPathName = FTopLevelAssetPath("/Script/EasyEditorExtend","DynamicComboButtonExtend");
		
		AssetRegistryModule.Get().GetAssetsByClass(ClassPathName, AssetData,true);
		for (int i = 0; i < AssetData.Num(); i++) {
			UDynamicComboButtonExtend* FoundAsset = Cast<UDynamicComboButtonExtend>(AssetData[i].GetAsset());
			
			if (FoundAsset != NULL) {
				Items.Add(FoundAsset);
			}
		}
		return Items;
	}

};
