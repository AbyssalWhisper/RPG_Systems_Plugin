// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/InventorySystem/RPG_ItemData.h"

FText URPG_ItemData::GetCategoryText()
{
	if (ItemType)
	{
		return ItemType->GetCategoryText();
	}
    return FText();
}

FText URPG_ItemData::GetSubCategoryText()
{
	if (ItemType)
	{
		return ItemType->GetSubCategoryText();
	}
    return FText();
}
