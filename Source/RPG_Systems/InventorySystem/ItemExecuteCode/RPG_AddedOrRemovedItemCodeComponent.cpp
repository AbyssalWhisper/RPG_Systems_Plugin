// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/InventorySystem/ItemExecuteCode/RPG_AddedOrRemovedItemCodeComponent.h"

#include "RPG_Systems/InventorySystem/RPG_InventoryComponent.h"

bool URPG_AddedOrRemovedItemCodeComponent::Execute_Implementation(AActor* OwnerActor, URPG_InventoryComponent* OwnerInventory, URPG_ItemData* Item_, int Count_, ESlotOp SlotOp)
{
	return false;
}
