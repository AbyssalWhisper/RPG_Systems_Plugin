// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RPG_Systems/SaveGame/RPG_SaveGame_Library.h"
#include "RPG_MapDataSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_MapDataSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FGuid, FRPG_ChestData> WorldChests;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
			TMap<FGuid, FRPG_ChestData> PlayersChests;
		 
		double GameTimePlayed;
};
