// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultToInstancedObject.h"
#include "RPG_AddedOrRemovedItemCodeComponent.generated.h"

/**
 * 
 */
class URPG_ItemData;
UCLASS(Abstract)
class RPG_SYSTEMS_API URPG_AddedOrRemovedItemCodeComponent : public UDefaultToInstancedObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Execute(AActor* OwnerActor, URPG_InventoryComponent* OwnerInventory, URPG_ItemData* Item_, int Count_);

};
