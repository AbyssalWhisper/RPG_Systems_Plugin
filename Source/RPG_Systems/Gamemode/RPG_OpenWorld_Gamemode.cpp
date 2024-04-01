// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Gamemode/RPG_OpenWorld_Gamemode.h"
#include "Kismet/GameplayStatics.h"
#include "RPG_Systems/SaveGame/RPG_MapDataSaveGame.h"
#include "RPG_Systems/InteractiveObjects/RPG_Base_Containers.h"
#include "RPG_Systems/InventorySystem/RPG_InventoryComponent.h"
#include "Containers/Map.h"
#include "Misc/Guid.h"
 


void ARPG_OpenWorld_Gamemode::BeginPlay() {
	Super::BeginPlay();
	
}

void ARPG_OpenWorld_Gamemode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (HasAuthority())
	{
		SaveGame();
	}
	
}

void ARPG_OpenWorld_Gamemode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GameTimePlayed += DeltaTime;
}

void ARPG_OpenWorld_Gamemode::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(UGameplayStatics::GetCurrentLevelName(GetWorld(), true)+"/MapData", 0))
	{
		USaveGame* Save = UGameplayStatics::LoadGameFromSlot(UGameplayStatics::GetCurrentLevelName(GetWorld(), true) + "/MapData", 0);
		MapDataSaveGame = Cast<URPG_MapDataSaveGame>(Save);
		if (!MapDataSaveGame)
		{
			Save = UGameplayStatics::CreateSaveGameObject(URPG_MapDataSaveGame::StaticClass());
			MapDataSaveGame = Cast<URPG_MapDataSaveGame>(Save);
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "Save Game Created");
		}
	}
	else
	{
		USaveGame* Save = UGameplayStatics::CreateSaveGameObject(URPG_MapDataSaveGame::StaticClass());
		MapDataSaveGame = Cast<URPG_MapDataSaveGame>(Save);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "Save Game Created");
	}
	
	// Load Player Chests
	LoadPlayerChests();
	//WorldChest
	LoadWorldChests();
}

void ARPG_OpenWorld_Gamemode::LoadPlayerChests()
{
	for (auto& List : MapDataSaveGame->PlayersChests)
	{
		if (!List.Value.ContainerClass)return;
		FTransform transform(List.Value.WorldRotation, List.Value.WorldLocation,FVector(1,1,1));
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ARPG_Base_Containers* SpawnActor = GetWorld()->SpawnActorDeferred<ARPG_Base_Containers>(List.Value.ContainerClass, transform);

		if (SpawnActor)
		{
			SpawnActor->ActorWorldGuid = List.Key;
			SpawnActor->ContainerInventory->Items = List.Value.Items;
			SpawnActor->FinishSpawning(transform);

		}
	}
}

void ARPG_OpenWorld_Gamemode::LoadWorldChests()
{
	for (ARPG_Base_Containers* Container : WorldChests)
	{
		if (Container)
		{
			FRPG_ChestData* Data = MapDataSaveGame->WorldChests.Find(Container->GetGuid());
			if (Data)
			{
				Container->ContainerInventory->Items = Data->Items;
			}
		}
	}
}

void ARPG_OpenWorld_Gamemode::SaveGame()
{
	if (MapDataSaveGame)
	{
	
		for (ARPG_Base_Containers * Container : PlayersChests)
		{
			if (Container)
			{
				MapDataSaveGame->PlayersChests.Add(Container->ActorWorldGuid, FRPG_ChestData(Container->GetActorLocation(), Container->GetActorRotation(), Container->ContainerInventory->Items,0, Container->GetClass()));
				
			}
		}
		for (ARPG_Base_Containers* Container : WorldChests)
		{
			if (Container)
			{
				MapDataSaveGame->WorldChests.Add(Container->GetGuid(), FRPG_ChestData(Container->GetActorLocation(), Container->GetActorRotation(), Container->ContainerInventory->Items, 0, Container->GetClass()));
				
			}
		}
	 
		UGameplayStatics::SaveGameToSlot(MapDataSaveGame, UGameplayStatics::GetCurrentLevelName(GetWorld(), true) + "/MapData", 0);
		UE_LOG(LogTemp, Warning, TEXT("Salvando jogo"));
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "Salvando jogo");
	}
}

void ARPG_OpenWorld_Gamemode::LoadPlayersBuilds()
{
}

void ARPG_OpenWorld_Gamemode::SpawnChest(FTransform Transform)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ARPG_Base_Containers* SpawnActor = GetWorld()->SpawnActor<ARPG_Base_Containers>(ContainerClass, Transform);
}
