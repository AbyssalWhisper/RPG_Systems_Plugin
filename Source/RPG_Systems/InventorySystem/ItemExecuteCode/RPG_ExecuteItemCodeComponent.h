// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultToInstancedObject.h"
#include "RPG_Systems/InventorySystem/RPG_InventoryComponent.h"
#include "RPG_ExecuteItemCodeComponent.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RPG_SYSTEMS_API URPG_ExecuteItemCodeComponent : public UDefaultToInstancedObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ExecuteOnServer(AActor* OwnerActor, URPG_InventoryComponent* OwnerInventory, int ItemSlot);

	// Only use this function for visual effects.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ExecuteOnClient(AActor* OwnerActor, URPG_InventoryComponent* OwnerInventory, int ItemSlot);

};
