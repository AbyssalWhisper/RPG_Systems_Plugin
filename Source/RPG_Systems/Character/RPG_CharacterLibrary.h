// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum E_RestingStates
{
	E_None     UMETA(DisplayName = "None"),
	E_Sitting      UMETA(DisplayName = "Sitting"),
	E_Laying  UMETA(DisplayName = "Laying"),
	E_Sleeping  UMETA(DisplayName = "Sleeping"),
};
UENUM(BlueprintType)
enum E_WalkMovementStates
{
	SlowWalk UMETA(DisplayName = "SlowWalk"),
	Walk      UMETA(DisplayName = "Walk"),
	Run  UMETA(DisplayName = "Run"),
	Sprint  UMETA(DisplayName = "Sprint"),
};

UENUM(BlueprintType)
enum E_Gender
{
	Male UMETA(DisplayName = "Male"),
	Female      UMETA(DisplayName = "Female"),
};
/**
 * 
 */
class RPG_SYSTEMS_API RPG_CharacterLibrary
{
public:
	RPG_CharacterLibrary();
	~RPG_CharacterLibrary();
};
