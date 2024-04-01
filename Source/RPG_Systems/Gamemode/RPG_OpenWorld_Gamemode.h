// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPG_OpenWorld_Gamemode.generated.h"

class URPG_MapDataSaveGame;
class ARPG_Base_Containers;
/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API ARPG_OpenWorld_Gamemode : public AGameModeBase
{
	GENERATED_BODY()
public: 
		double GameTimePlayed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URPG_MapDataSaveGame* MapDataSaveGame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ARPG_Base_Containers> ContainerClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ARPG_Base_Containers*> PlayersChests;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ARPG_Base_Containers*> WorldChests;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void LoadGame();
		void LoadPlayerChests();
		void LoadWorldChests();
	UFUNCTION(BlueprintCallable)
	void SaveGame();
	void LoadPlayersBuilds();
	UFUNCTION(BlueprintCallable)
	void SpawnChest(FTransform Transform);
};
