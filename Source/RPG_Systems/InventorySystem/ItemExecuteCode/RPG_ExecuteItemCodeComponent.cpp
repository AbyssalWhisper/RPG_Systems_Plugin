// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/InventorySystem/ItemExecuteCode/RPG_ExecuteItemCodeComponent.h"


bool URPG_ExecuteItemCodeComponent::ExecuteOnServer_Implementation(AActor* OwnerActor, URPG_InventoryComponent* OwnerInventory, int ItemSlot)
{
	return false;
}

bool URPG_ExecuteItemCodeComponent::ExecuteOnClient_Implementation(AActor* OwnerActor, URPG_InventoryComponent* OwnerInventory, int ItemSlot)
{
	return false;
}

