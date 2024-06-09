// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPG_ItemSubCategory.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable,BlueprintType,DefaultToInstanced,EditInlineNew,CollapseCategories)
class RPG_SYSTEMS_API URPG_ItemSubCategory : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	FText GetSubCategoryText();
};
