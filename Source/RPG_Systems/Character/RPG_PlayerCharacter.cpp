// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Character/RPG_PlayerCharacter.h"
#include "RPG_Systems/InventorySystem/RPG_InventoryComponent.h"
#include "RPG_Systems/PlayerController/RPG_PlayerController.h"

ARPG_PlayerCharacter::ARPG_PlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	Inventory = CreateDefaultSubobject<URPG_InventoryComponent>("Inventory");
}

void ARPG_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//ARPG_PlayerController* Player = Cast<ARPG_PlayerController>(NewController);
	//if (Player) {
	//	Inventory->AddPlayer(Player);
	//}
	
}

void ARPG_PlayerCharacter::UnPossessed()
{
	ARPG_PlayerController* Player = Cast<ARPG_PlayerController>(Controller);
	if (Player) {
		Inventory->RemovePlayer(Player);
	}
}
