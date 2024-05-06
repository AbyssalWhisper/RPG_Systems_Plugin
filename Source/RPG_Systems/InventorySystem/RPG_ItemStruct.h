// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "ItemTypes/RPG_BaseItemType.h"
#include "ModifierOp/RPG_ItemModifierOp.h"
#include "RPG_ItemStruct.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class URPG_ItemData;
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
struct  FSTR_RPG_GameplayAttributeApply : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,Instanced, BlueprintReadWrite, meta = (ShowOnlyInnerProperties))
	TObjectPtr<URPG_ItemModifierOp> ModifierOp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Magnitude = 0;
	FSTR_RPG_GameplayAttributeApply() {
		Magnitude = 0;
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
	UPROPERTY(EditAnywhere,Instanced, BlueprintReadWrite, meta = (ShowOnlyInnerProperties, DisplayPriority = 0))
		TObjectPtr<URPG_BaseItemType> ItemType;
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
		URPG_ItemData* Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Count = 0;

	FSTR_RPG_ItemSlot() {
		Item = nullptr;
		Count = 0;
	}

	FSTR_RPG_ItemSlot(URPG_ItemData* Item_, int Count_) {
		Item = Item_;
		Count = Count_;
	}
};


USTRUCT(BlueprintType)
struct  FSTR_RPG_LootItems : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		URPG_ItemData* Item;
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
