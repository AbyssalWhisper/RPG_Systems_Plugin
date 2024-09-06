// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "RPG_CharacterMoverComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType,meta=(BlueprintSpawnableComponent))
class RPG_SYSTEMS_API URPG_CharacterMoverComponent : public UCharacterMoverComponent
{
	GENERATED_BODY()
	
public:

	URPG_CharacterMoverComponent();
};
