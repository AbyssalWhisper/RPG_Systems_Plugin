// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RPG_UseItemConditionComponent.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable,BlueprintType,DefaultToInstanced,EditInlineNew,CollapseCategories)
class RPG_SYSTEMS_API URPG_UseItemConditionComponent : public UObject
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable,BlueprintPure,BlueprintNativeEvent)
	FText GetCantUseMensage();
	UFUNCTION(BlueprintCallable,BlueprintPure,BlueprintNativeEvent)
	bool CanUse(AActor* Owner);
};
