// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "UObject/NoExportTypes.h"
#include "RPG_BaseItemType.generated.h"

/**
 * 
 */
class URPG_ItemSubCategory;
class URPG_InventoryComponent;
UCLASS(Abstract,Blueprintable,BlueprintType,DefaultToInstanced,EditInlineNew,CollapseCategories)
class RPG_SYSTEMS_API URPG_BaseItemType : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	FText GetCategoryText();
	UFUNCTION(BlueprintCallable)
	FText GetSubCategoryText();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	URPG_ItemSubCategory* SubCategory;
};
