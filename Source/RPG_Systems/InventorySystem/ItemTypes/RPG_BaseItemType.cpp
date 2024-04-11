// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/InventorySystem/ItemTypes/RPG_BaseItemType.h"

bool URPG_BaseItemType::ExecuteOnServer_Implementation(AActor* OwnerActor, URPG_InventoryComponent* OwnerInventory,
	int ItemSlot)
{
	return false;
}

bool URPG_BaseItemType::ExecuteOnClient_Implementation(AActor* OwnerActor, URPG_InventoryComponent* OwnerInventory,
	int ItemSlot)
{
	return false;
}
