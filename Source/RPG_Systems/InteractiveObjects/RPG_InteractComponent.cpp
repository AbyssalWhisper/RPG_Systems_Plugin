// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/InteractiveObjects/RPG_InteractComponent.h"
#include "RPG_Systems/General_Interfaces/RPG_Interact_Interface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG_Systems/InventorySystem/RPG_InventoryComponent.h"
#include "RPG_Systems/InventorySystem/RPG_MasterItem.h"
#include "RPG_Systems/Character/RPG_BaseCharacter.h"
#include "RPG_Systems/Character/RPG_PlayerCharacter.h"
#include "RPG_Systems/InventorySystem/RPG_MasterItem.h"
#include "RPG_Systems/PlayerController/RPG_PlayerController.h"

// Sets default values for this component's properties
URPG_InteractComponent::URPG_InteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URPG_InteractComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ARPG_PlayerController>(GetOwner());
	// ...
	
}


// Called every frame
void URPG_InteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (GetOwner()->HasAuthority() && CurrentContainer && PlayerController && PlayerController->GetPawn())
	{
		float VectorSize = (CurrentContainer->GetOwner()->GetActorLocation() - PlayerController->GetPawn()->GetActorLocation()).Size();

		if (VectorSize > 250)
		{
			 
			CurrentContainer->RemovePlayer(PlayerController);
			CurrentContainer = nullptr;
			PlayerController->Client_CloseContainer();
		}
		
	}

	if (PlayerController && PlayerController->GetPawn())
	{
		if (PlayerController->IsLocalController())
		{
			if (bInteractTrace)
			{
				TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
				ObjectTypesArray.Reserve(1);
				ObjectTypesArray.Emplace(ECollisionChannel::ECC_Visibility);
				TArray<AActor*> IgnoreActors;
				IgnoreActors.Add(PlayerController->GetPawn());
				FHitResult hit;
				bool Sucess = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), PlayerController->PlayerCameraManager->GetCameraLocation(), PlayerController->PlayerCameraManager->GetCameraLocation() + (UKismetMathLibrary::GetForwardVector(PlayerController->PlayerCameraManager->GetCameraRotation()) * 1000), 10.f, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, IgnoreActors, EDrawDebugTrace::None, hit, true);

				if (Sucess) {

					if ((hit.Location - PlayerController->GetPawn()->GetActorLocation()).Size() < 250.f)
					{

						SetInteractTarget(hit.GetActor());
					}
					else
					{
						SetInteractTarget(nullptr);
					}

				}
				else
				{
					SetInteractTarget(nullptr);

				}

			}
		}
	}
	// ...
}

void URPG_InteractComponent::Interact()
{
	if (InteractTarget && InteractTarget->GetClass()->ImplementsInterface(URPG_Interact_Interface::StaticClass()))
	{
		//Server_Interact(InteractTarget);
		IRPG_Interact_Interface::Execute_Interact(InteractTarget, PlayerController);
	}
}

void URPG_InteractComponent::SetInteractTarget(AActor* InteractActor)
{
	if (InteractActor != InteractTarget)
	{
		InteractTarget = InteractActor;
		OnInteractTargetActorChange.Broadcast(InteractTarget);
	}
}

bool URPG_InteractComponent::Server_Interact_Validate(AActor* TargetActor)
{
	return true;
}

void URPG_InteractComponent::Server_Interact_Implementation(AActor* TargetActor)
{
	if (TargetActor && TargetActor->GetClass()->ImplementsInterface(URPG_Interact_Interface::StaticClass()))
	{

		IRPG_Interact_Interface::Execute_Interact(TargetActor, PlayerController);
	}
}

void URPG_InteractComponent::Client_UpdateInventorySlot_Implementation(URPG_InventoryComponent* Inventory, FSTR_RPG_ItemSlot Item, int SlotIndex)
{
	if (Inventory)
	{
		Inventory->SetItemSlotOnClient(Item, SlotIndex);
	}

}

void URPG_InteractComponent::Client_UpdateAllSlotsContainer_Implementation(URPG_InventoryComponent* Inventory, const TArray<FSTR_RPG_ItemSlot>& Items)
{
	if (Inventory)
	{
		Inventory->Items = Items;
	}
}
 

bool URPG_InteractComponent::TryPickupItemOnServer_Validate(ARPG_MasterItem* Item)
{
	return true;
}

void URPG_InteractComponent::TryPickupItemOnServer_Implementation(ARPG_MasterItem* ItemActor)
{
	bool Sucess = false;
	if (ItemActor)
	{
		ARPG_PlayerCharacter* PlayerCharacter  = Cast<ARPG_PlayerCharacter>(PlayerController->GetPawn());
		if (PlayerCharacter)
		{
			FSTR_RPG_ItemSlot RemainingItem;

			if (!PlayerCharacter->Inventory->TryAddItem(ItemActor->Item.Item,ItemActor->Item.Count, RemainingItem))return;

			if (RemainingItem.Count>0)
			{
				ItemActor->Item = RemainingItem;
			}
			else
			{
				ItemActor->Destroy();
			}
		}
	}
}

bool URPG_InteractComponent::Server_TransferItem_Validate(int SlotIndex, URPG_InventoryComponent* Container1, int Amount, int TargetSlotIndex, URPG_InventoryComponent* Container2)
{
	return true;
}

void URPG_InteractComponent::Server_TransferItem_Implementation(int SlotIndex, URPG_InventoryComponent* Container1,int Amount, int TargetSlotIndex, URPG_InventoryComponent* Container2)
{
	if (Container1 && Container2)
	{
		Container1->TransferItem(SlotIndex, Container1, Amount, TargetSlotIndex, Container2);
	}
}

void URPG_InteractComponent::SetContainer(URPG_InventoryComponent* Container)
{
	if (!Container)return;
	if (Container != CurrentContainer) {
		if (CurrentContainer)
		{
			CurrentContainer->RemovePlayer(PlayerController);
		}
		CurrentContainer = Container;
		CurrentContainer->AddPlayer(PlayerController);
	}
}

void URPG_InteractComponent::InitContainerInventory_Implementation(URPG_InventoryComponent* Inventory)
{
	if (Inventory)
	{
		Inventory->InitContainer();
	}
}




