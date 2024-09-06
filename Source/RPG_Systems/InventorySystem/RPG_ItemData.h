
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ItemExecuteCode/RPG_ExecuteItemCodeComponent.h"
#include "RPG_Systems/InventorySystem/RPG_ItemStruct.h"

#include "RPG_ItemData.generated.h"


/**
 * 
 */
class UGameplayAbility;
class UGameplayEffect;
class URPG_UseItemConditionComponent;
class URPG_ItemDropSound;
class URPG_AddedOrRemovedItemCodeComponent;
UCLASS()
class RPG_SYSTEMS_API URPG_ItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name = FText::GetEmpty();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description = FText::GetEmpty();
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxCount = 999;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryEvents")
	TArray<TObjectPtr<URPG_ExecuteItemCodeComponent>> OnUseItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryEvents")
	TArray<TObjectPtr<URPG_AddedOrRemovedItemCodeComponent>> OnItemAddedToInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryEvents")
	TArray<TObjectPtr<URPG_AddedOrRemovedItemCodeComponent>> OnItemRemovedFromInventory;


	UPROPERTY(EditAnywhere,Instanced, BlueprintReadWrite, meta = (ShowOnlyInnerProperties))
	TArray<TObjectPtr<URPG_UseItemConditionComponent>> UseItemConditions;
	UPROPERTY(EditAnywhere,Instanced, BlueprintReadWrite, meta = (ShowOnlyInnerProperties))
	TObjectPtr<URPG_BaseItemType> ItemType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Decay")
	bool bCanDecay = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (EditConditionHides, EditCondition = "bCanDecay"),Category="Decay")
	float DecayTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (EditConditionHides, EditCondition = "bCanDecay"),Category="Decay")
	TSoftObjectPtr<URPG_ItemData> DecayItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (EditConditionHides, EditCondition = "bCanDrop"),Category="Drop")
	bool bCanDrop = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (EditConditionHides, EditCondition = "bCanDrop"),Category="Drop")
	TSoftObjectPtr<UStaticMesh> DropMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,meta = (EditConditionHides, EditCondition = "bCanDrop"),Category="Drop")
	TSoftObjectPtr<URPG_ItemDropSound> DropSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> TakeSound;

	UFUNCTION(BlueprintCallable)
	FText GetCategoryText();
	UFUNCTION(BlueprintCallable)
	FText GetSubCategoryText();
	
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
