// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/InventorySystem/ItemTypes/RPG_BaseItemType.h"
#include "RPG_Systems/InventorySystem/ItemTypes/ItemSubCategory/RPG_ItemSubCategory.h"

FText URPG_BaseItemType::GetCategoryText_Implementation()
{
	return FText();
}

FText URPG_BaseItemType::GetSubCategoryText()
{
	if (SubCategory)
	{
		return SubCategory->GetSubCategoryText();
	}
	return FText();
}
