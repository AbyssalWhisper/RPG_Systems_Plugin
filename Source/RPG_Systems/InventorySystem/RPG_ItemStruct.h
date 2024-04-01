// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "RPG_ItemStruct.generated.h"

class UGameplayAbility;
class UGameplayEffect;
USTRUCT(BlueprintType)
struct  FSTR_RPG_Effects_Data : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UGameplayEffect> Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Value;
	FSTR_RPG_Effects_Data() {
		Effect = nullptr;
		Value = 1;
	}
};

USTRUCT(BlueprintType)
struct  FSTR_RPG_ItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName = FText::GetEmpty();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;
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
	FSTR_RPG_ItemData() {
		ItemName = FText::GetEmpty();
		ItemID = "";
		Texture = nullptr;
		MaxCount = 999;
		Ability = nullptr;
	}
};

USTRUCT(BlueprintType)
struct  FSTR_RPG_ItemSlot : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Count = 0;

	FSTR_RPG_ItemSlot() {
		ItemID = "";
		Count = 0;
	}

	FSTR_RPG_ItemSlot(FName ItemID_, int Count_) {
		ItemID = ItemID_;
		Count = Count_;
	}
};


USTRUCT(BlueprintType)
struct  FSTR_RPG_LootItems : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FDataTableRowHandle Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DropChance = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MinCount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxCount = 1;

};

USTRUCT(BlueprintType)
struct  FSTR_RPG_LootLists : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSTR_RPG_LootItems> Items;
};
/**
 * 
 */
class RPG_SYSTEMS_API RPG_ItemStruct
{
public:
	RPG_ItemStruct();
	~RPG_ItemStruct();


};
