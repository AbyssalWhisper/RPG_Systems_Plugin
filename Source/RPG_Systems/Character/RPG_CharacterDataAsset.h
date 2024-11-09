// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RPG_Systems/Character/RPG_CharacterLibrary.h"
#include "GameplayTagContainer.h"
#include "RPG_Systems/InventorySystem/ItemTypes/RPG_BaseItemType.h"
#include "RPG_CharacterDataAsset.generated.h"

UENUM(BlueprintType)
enum E_Diet
{
	Carnivore     UMETA(DisplayName = "Carnivore"),
	Herbivore      UMETA(DisplayName = "Herbivore"),
	Omnivorous  UMETA(DisplayName = "Omnivorous"),
};

 
/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_CharacterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayTag Breed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<E_Diet> Diet;

#pragma region Attributes

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Health")
		float MaxHealth = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Health")
		float HealthRegenRate = 1;

	//Atributos da Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Stamina")
		float MaxStamina = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Stamina")
		float StaminaRegenRate = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Stamina")
		float StaminaRegenTickInterval = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Mana")
		float MaxMana = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Mana")
		float ManaRegenRate = 1;
	//Atributos da Fome
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Food")
		float MaxFood = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Food")
		float FoodRegenRate = 1;

	//Atributos da Sede
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Thirsty")
		float MaxThirst = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Thirsty")
		float ThirstRegenRate = 1;

	//Atributos do Oxigenio
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Oxygen")
		float MaxOxygen = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Oxygen")
		float OxygenRegenRate = 1;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float Attack = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float Armor = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float MagicArmor = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float DMG_Fire_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float DMG_Earth_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float DMG_Wind_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float DMG_Water_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float DMG_Poison_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float DMG_Holy_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float DMG_Shadow_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float DMG_Ghost_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float DMG_Undead_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System|Fall")
		float FallDamage_Multiplier = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System|Fall")
		float FallVelocityInitDamage = 600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float FoodDamage = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float FoodDamageTickInterval = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float ThirstDamage = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Damage System")
		float ThirstDamageTickInterval = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Point System")
		float MaxHealthPerPoint = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Point System")
		float MaxStaminaPerPoint = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Point System")
		float MaxOxygenPerPoint = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Point System")
		float MaxFoodPerPoint = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Point System")
		float MaxWaterPerPoint = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Point System")
		float AttackPercentPerPoint = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Point System")
		float ArmorPercentPerPoint = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Point System")
		float EarnPointsTime = 12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Point System")
		int MaxAttributePoints = 50;
#pragma endregion Attributes

#pragma region Movement
		// Walk Move Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MoveSpeed")
		float SlowWalkMoveSpeed = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MoveSpeed")
		float WalkMoveSpeed = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MoveSpeed")
		float RunMoveSpeed = 350;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MoveSpeed")
		float SprintMoveSpeed = 450;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MoveSpeed")
		float FlySpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MoveSpeed")
		float FlyFastSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MoveSpeed")
		float FlySprintSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MoveSpeed")
		float SwimMoveSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MoveSpeed")
		float SwimFastMoveSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|MoveSpeed")
		float SwimSprintMoveSpeed = 200;


	//Max Speed Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AnimMoveSpeed")
		float AnimSlowWalkMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AnimMoveSpeed")
		float AnimWalkMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AnimMoveSpeed")
		float AnimRunMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AnimMoveSpeed")
		float AnimSprintMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AnimMoveSpeed")
		float AnimFlySpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AnimMoveSpeed")
		float AnimFlyFastSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AnimMoveSpeed")
		float AnimFlySprintSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AnimMoveSpeed")
		float AnimSwimMoveSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AnimMoveSpeed")
		float AnimSwimFastMoveSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|AnimMoveSpeed")
		float AnimSwimSprintMoveSpeed = 200;
#pragma endregion Movement

#pragma region Abilities
	//Abilities Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Abilities")
		TArray<TSubclassOf<class URPG_GameplayAbility>> CharacterAbilities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TArray<TSubclassOf<class UGameplayEffect>> Effects;
#pragma endregion Abilities

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CarrySystem")
		float MinBodyWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CarrySystem")
		float MaxBodyWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CarrySystem")
		float CarryWeightMultiplier = 1;
};
