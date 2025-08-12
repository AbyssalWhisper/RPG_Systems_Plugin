// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetRegistry/AssetRegistryModule.h"
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
	FName ButtonName = "MyButtonNameId";
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText ButtonTooltip = FText::FromString("MyButton Tooltip");
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FText ButtonLabel = FText::FromString("MyButton");
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Instanced,meta = (ShowOnlyInnerProperties))
	UEasyEditorObjectExecuteCode* CustomExecuteCode;
	UFUNCTION()
	void Execute();

	UFUNCTION(BlueprintCallable, Category = "DynamicButtonEditorExtend")
	static void CreateDymamicButtons();
	static void CreateButton(UDynamicButtonEditorExtend* ButtonObject);

	static TArray<UDynamicButtonEditorExtend*> GetAllGetAllDynamicButtonExtend()
	{
		TArray<UDynamicButtonEditorExtend*> Items;
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		TArray<FAssetData> AssetData;
		FTopLevelAssetPath ClassPathName = FTopLevelAssetPath("/Script/EasyEditorExtend","DynamicButtonEditorExtend");
		
		AssetRegistryModule.Get().GetAssetsByClass(ClassPathName, AssetData,true);
		UE_LOG(LogTemp, Warning, TEXT("encontrados %s  "), *FString::FromInt(AssetData.Num()));
		for (int i = 0; i < AssetData.Num(); i++) {
			UDynamicButtonEditorExtend* FoundAsset = Cast<UDynamicButtonEditorExtend>(AssetData[i].GetAsset());
			
			if (FoundAsset != NULL) {
				Items.Add(FoundAsset);
			}
		} 
		return Items;
	}

};
