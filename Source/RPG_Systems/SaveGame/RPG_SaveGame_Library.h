// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "RPG_Systems/InventorySystem/RPG_ItemStruct.h"
#include "Runtime/Launch/Resources/Version.h"
#include "RPG_SaveGame_Library.generated.h"

class ARPG_Base_Containers;
USTRUCT(BlueprintType)
struct  FRPG_ChestData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector WorldLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator WorldRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSTR_RPG_ItemSlot> Items;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ARPG_Base_Containers> ContainerClass = nullptr;
 
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		double LastSpawnLootTime;
 
 


		
	FRPG_ChestData() {
		WorldLocation = FVector::ZeroVector;
		WorldRotation = FRotator::ZeroRotator;
		Items.SetNum(0);
		LastSpawnLootTime = 0;
		ContainerClass = nullptr;
	}
	FRPG_ChestData(FVector WorldLocation_, FRotator WorldRotation_, TArray<FSTR_RPG_ItemSlot> Items_, double LastSpawnLootTime_ , TSubclassOf<ARPG_Base_Containers> ContainerClass_) {
		WorldLocation = WorldLocation_;
		WorldRotation = WorldRotation_;
		Items = Items_;
		LastSpawnLootTime = LastSpawnLootTime_;
		ContainerClass = ContainerClass_;
	}
};

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_SaveGame_Library : public UObject
{
	GENERATED_BODY()
	
};
