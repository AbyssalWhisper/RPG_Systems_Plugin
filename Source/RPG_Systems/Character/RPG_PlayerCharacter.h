// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Systems/Character/RPG_BaseCharacter.h"
#include "RPG_PlayerCharacter.generated.h"

/**
 * 
 */

class URPG_InventoryComponent;
UCLASS()
class RPG_SYSTEMS_API ARPG_PlayerCharacter : public ARPG_BaseCharacter
{
	GENERATED_BODY()
public:
	ARPG_PlayerCharacter(const FObjectInitializer& ObjectInitializer);
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		URPG_InventoryComponent* Inventory;
		void PossessedBy(AController* NewController) override;
		virtual void UnPossessed() override;
};
