// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum E_RPG_PhysicalDamage
{
	Cut      UMETA(DisplayName = "Cut"),
	
};

UENUM(BlueprintType)
enum E_RPG_MagicalDamage
{
	Fire UMETA(DisplayName = "Fire"),
	Earth UMETA(DisplayName = "Earth"),
	Wind UMETA(DisplayName = "Wind"),
	Water UMETA(DisplayName = "Water"),
	Poison UMETA(DisplayName = "FPoisonire"),
	Holy UMETA(DisplayName = "Holy"),
	Shadow UMETA(DisplayName = "Shadow"),
	Ghost UMETA(DisplayName = "Ghost"),
	Undead UMETA(DisplayName = "Undead"),
};
/**
 * 
 */
class RPG_SYSTEMS_API RPG_DamageTypeEnum
{
public:
	RPG_DamageTypeEnum();
	~RPG_DamageTypeEnum();
};
