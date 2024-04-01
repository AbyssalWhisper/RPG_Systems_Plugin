// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/PlayerController/RPG_PlayerController.h"
#include "RPG_Systems/General_Interfaces/RPG_Interact_Interface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG_Systems/InteractiveObjects/RPG_InteractComponent.h"
#include "Net/UnrealNetwork.h" 
#include "RPG_Systems/InteractiveObjects/RPG_Base_Containers.h"

ARPG_PlayerController::ARPG_PlayerController() {

	InteractionManager = CreateDefaultSubobject<URPG_InteractComponent>("InteractionManagerComponent");
	InteractionManager->SetIsReplicated(true);
}

void ARPG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ARPG_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void ARPG_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("InteractAction", IE_Pressed, this, &ARPG_PlayerController::Interact);
}

void ARPG_PlayerController::Interact()
{
	InteractionManager->Interact();
}

void ARPG_PlayerController::Open_Container_Implementation(URPG_InventoryComponent* Container)
{
	Server_OpenContainer(Container);
}

void ARPG_PlayerController::Try_PickupItem_Implementation(ARPG_MasterItem* Item)
{
	if (Item)
	{
		InteractionManager->TryPickupItemOnServer(Item);
	}
}

bool ARPG_PlayerController::Server_OpenContainer_Validate(URPG_InventoryComponent* Container)
{
	return true;
}

void ARPG_PlayerController::Server_OpenContainer_Implementation(URPG_InventoryComponent* Container)
{
	if (Container)
	{
		InteractionManager->Client_UpdateAllSlotsContainer(Container, Container->Items);
		Client_OpenContainer(Container);
		InteractionManager->SetContainer(Container);
	}
}

void ARPG_PlayerController::Client_OpenContainer_Implementation(URPG_InventoryComponent* Container)
{
	OpenHUDContainer(Container);
}

void ARPG_PlayerController::OpenHUDContainer_Implementation(URPG_InventoryComponent* Container)
{
}

void ARPG_PlayerController::Client_CloseContainer_Implementation()
{
	BP_CloseContainer();
}

void ARPG_PlayerController::BP_CloseContainer_Implementation()
{
}

