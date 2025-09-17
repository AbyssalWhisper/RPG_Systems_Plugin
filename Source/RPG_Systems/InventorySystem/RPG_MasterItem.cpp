// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/InventorySystem/RPG_MasterItem.h"

#include "RPG_ItemData.h"
#include "RPG_Systems/General_Interfaces/RPG_PlayerInterface.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARPG_MasterItem::ARPG_MasterItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetSimulatePhysics(true);
	bReplicates = true;
}

// Called when the game starts or when spawned
void ARPG_MasterItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPG_MasterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPG_MasterItem::Interact_Implementation(APlayerController* PlayerController)
{
	if (PlayerController  && PlayerController->GetClass()->ImplementsInterface(URPG_PlayerInterface::StaticClass()))
	{
		IRPG_PlayerInterface::Execute_Try_PickupItem(PlayerController, this);
		
	}
}

void ARPG_MasterItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ARPG_MasterItem, Item);
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARPG_MasterItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (Item.Item)
	{
		auto ItemMesh = Item.Item->DropMesh.LoadSynchronous();
		if (ItemMesh)
		{
			Mesh->SetStaticMesh(ItemMesh);
		}
	}
}

FText ARPG_MasterItem::GetInteractText_Implementation()
{
	if (Item.Item)
	{
		return Item.Item->Name;
	}
	return FText::GetEmpty();
}

