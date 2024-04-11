// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/InteractiveObjects/RPG_Base_Containers.h"
#include "RPG_Systems/InventorySystem/RPG_InventoryComponent.h"
#include "RPG_Systems/PlayerController/RPG_PlayerController.h"
#include "RPG_Systems/InventorySystem/RPG_ItemStruct.h"
#include "RPG_Systems/Gamemode/RPG_OpenWorld_Gamemode.h"
#include "Kismet/GameplayStatics.h"
#include "RPG_Systems/SaveGame/RPG_MapDataSaveGame.h"

// Sets default values
ARPG_Base_Containers::ARPG_Base_Containers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
	ContainerInventory = CreateDefaultSubobject<URPG_InventoryComponent>(TEXT("ContainerComp"));
	ContainerInventory->Slots = 15;
	bReplicates = true;
	//SkeletalMesh->SetupAttachment(RootComponent);
	// Construct Datatable
	
}

// Called when the game starts or when spawned
void ARPG_Base_Containers::BeginPlay()
{
	Super::BeginPlay();
 
	
	if (HasAuthority())
	{
		ARPG_OpenWorld_Gamemode* Gamemode = Cast<ARPG_OpenWorld_Gamemode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (!Gamemode)return;


		if (bNetStartup) {
			
			Gamemode->WorldChests.AddUnique(this);
				if (Gamemode->MapDataSaveGame) {
					FRPG_ChestData* Data = Gamemode->MapDataSaveGame->WorldChests.Find( GetGuid());
					if (Data)
					{
						ContainerInventory->Items = Data->Items;
						
						return;
					}

				}
			

			if (LootList.DataTable)
			{
				FSTR_RPG_LootLists* LootData = LootList.DataTable->FindRow<FSTR_RPG_LootLists>(LootList.RowName, "");
				if (LootData)
				{
					int Size = LootData->Items.Num();
					FSTR_RPG_ItemSlot RemainingItem;
					for (int i = 0; i < Size; i++)
					{
						if (FMath::FRand() <= LootData->Items[i].DropChance)
						{
							if (LootData->Items[i].Item != nullptr)
							{

								ContainerInventory->TryAddItem(LootData->Items[i].Item, FMath::RandRange(LootData->Items[i].MinCount, LootData->Items[i].MaxCount), RemainingItem);
							}

						}

					}
				}
			}
		}
		else
		{
			
			Gamemode->PlayersChests.AddUnique(this);
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, ActorWorldGuid.ToString());
			if (ActorWorldGuid.IsValid())return;
		
			InitGuid();
			
		}
	}
}

void ARPG_Base_Containers::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HasAuthority())
	{
		ARPG_OpenWorld_Gamemode* Gamemode = Cast<ARPG_OpenWorld_Gamemode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (Gamemode)
		{
			if (bNetStartup)
			{
				Gamemode->WorldChests.Remove(this);
				if (Gamemode->MapDataSaveGame) {
					 
					Gamemode->MapDataSaveGame->WorldChests.Add(GetGuid(), FRPG_ChestData(GetActorLocation(), GetActorRotation(), ContainerInventory->Items, 0, this->GetClass()));
				}
			}
			else
			{
				Gamemode->PlayersChests.Remove(this);
			}
			 
		}
	}
}

void ARPG_Base_Containers::InitGuid()
{
	ActorWorldGuid = FGuid::NewGuid();
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, ActorWorldGuid.ToString());
	ARPG_OpenWorld_Gamemode* Gamemode = Cast<ARPG_OpenWorld_Gamemode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (Gamemode)
	{
		if (Gamemode->MapDataSaveGame) {
			
			if (Gamemode->MapDataSaveGame->PlayersChests.Contains(ActorWorldGuid)) {
				InitGuid();
			}
			else
			{
				Gamemode->MapDataSaveGame->PlayersChests.Add(ActorWorldGuid, FRPG_ChestData(GetActorLocation(), GetActorRotation(), ContainerInventory->Items, 0, this->GetClass()));
			}
		}
	}
}

FGuid ARPG_Base_Containers::GetGuid()
{
	return ActorWorldGuid;
}

// Called every frame
void ARPG_Base_Containers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 
}

void ARPG_Base_Containers::Interact_Implementation(APlayerController* PlayerController)
{
	ARPG_PlayerController* Player = Cast<ARPG_PlayerController>(PlayerController);
	if (Player)
	{
		Player->Server_OpenContainer(ContainerInventory);
	}
}

void ARPG_Base_Containers::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITOR
	ActorWorldGuid = ActorGuid;
#endif
}

