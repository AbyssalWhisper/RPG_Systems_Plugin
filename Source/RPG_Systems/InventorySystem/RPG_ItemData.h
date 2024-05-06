
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "RPG_Systems/InventorySystem/RPG_ItemStruct.h"
#include "RPG_ItemData.generated.h"

/**
 * 
 */
class UGameplayAbility;
class UGameplayEffect;

UCLASS()
class RPG_SYSTEMS_API URPG_ItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName = FText::GetEmpty();
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxCount = 999;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer Tags = FGameplayTagContainer();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> Ability;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSTR_RPG_Effects_Data> Effects;
	UPROPERTY(EditAnywhere,Instanced, BlueprintReadWrite, meta = (ShowOnlyInnerProperties))
	TObjectPtr<URPG_BaseItemType> ItemType;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static TArray<URPG_ItemData*> GetAllItems()
	{
		TArray<URPG_ItemData*> Items;
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		TArray<FAssetData> AssetData;
		FTopLevelAssetPath ClassPathName = FTopLevelAssetPath("/Script/RPG_Systems","RPG_ItemData");
		
		AssetRegistryModule.Get().GetAssetsByClass(ClassPathName, AssetData,true);
		for (int i = 0; i < AssetData.Num(); i++) {
			URPG_ItemData* FoundAsset = Cast<URPG_ItemData>(AssetData[i].GetAsset());
			
			if (FoundAsset != NULL) {
				Items.Add(FoundAsset);
			}
		}
		/*
		for (TObjectIterator<URPG_ItemData> It; It; ++It)
		{
			Items.Add(*It);
		}*/
		return Items;
	}
};
