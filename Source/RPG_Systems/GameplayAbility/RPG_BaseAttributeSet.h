// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPG_BaseAttributeSet.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class ARPG_BaseCharacter;

UCLASS()
class RPG_SYSTEMS_API URPG_BaseAttributeSet : public UAttributeSet
{
	

public:
	GENERATED_BODY()
		URPG_BaseAttributeSet();



	UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_Oxygen)
		FGameplayAttributeData Oxygen;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, Oxygen)
		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_MaxOxygen)
		FGameplayAttributeData MaxOxygen;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, MaxOxygen)
		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_OxygenRegenRate)
		FGameplayAttributeData OxygenRegenRate;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, OxygenRegenRate)

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_Food)
		FGameplayAttributeData 	Food;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, Food)
		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_MaxFood)
		FGameplayAttributeData MaxFood;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, MaxFood)
		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_FoodRegenRate)
		FGameplayAttributeData FoodRegenRate;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, FoodRegenRate)

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_Thirst)
		FGameplayAttributeData 	Thirst;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, Thirst)
		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_MaxThirst)
		FGameplayAttributeData MaxThirst;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, MaxThirst)
		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_ThirstRegenRate)
		FGameplayAttributeData ThirstRegenRate;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, ThirstRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_ExtraMoveSpeed)
		FGameplayAttributeData ExtraMoveSpeed;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, ExtraMoveSpeed)

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_MoveSpeedMultiplier)
		FGameplayAttributeData MoveSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, MoveSpeedMultiplier)

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_Stamina)
		FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, Stamina)

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_MaxStamina)
		FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, MaxStamina)
		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_StaminaRegenRate)
		FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, StaminaRegenRate)


		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_MaxMana)
		FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, MaxMana)
		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_Mana)
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, Mana)
		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_ManaRegenRate)
		FGameplayAttributeData ManaRegenRate;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, ManaRegenRate)

		void CalculateStamina(float& StaminaValue);
		void CalculateFood(float& FoodValue);
		void CalculateThirst(float& ThirstValue);
		void CalculateOxygen(float& OxygenValue);

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_Attack)
		FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, Attack)
		UFUNCTION()
		virtual void OnRep_Attack(const FGameplayAttributeData& OldAttack);

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_Armor)
		FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, Armor)

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_MagicArmor)
		FGameplayAttributeData MagicArmor;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, MagicArmor)
		UFUNCTION()
		virtual void OnRep_MagicArmor(const FGameplayAttributeData& OldMagicArmor);


		//Attributes
	

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_DMG_Fire_Multiplier)
		FGameplayAttributeData DMG_Fire_Multiplier;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, DMG_Fire_Multiplier)
		UFUNCTION()
		virtual void OnRep_DMG_Fire_Multiplier(const FGameplayAttributeData& OldDMG_Fire_Multiplier);

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_DMG_Earth_Multiplier)
		FGameplayAttributeData DMG_Earth_Multiplier;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, DMG_Earth_Multiplier)
		UFUNCTION()
		virtual void OnRep_DMG_Earth_Multiplier(const FGameplayAttributeData& OldDMG_Earth_Multiplier);

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_DMG_Wind_Multiplier)
		FGameplayAttributeData DMG_Wind_Multiplier;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, DMG_Wind_Multiplier)
		UFUNCTION()
		virtual void OnRep_DMG_Wind_Multiplier(const FGameplayAttributeData& OldDMG_Wind_Multiplier);

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_DMG_Water_Multiplier)
		FGameplayAttributeData DMG_Water_Multiplier;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, DMG_Water_Multiplier)
		UFUNCTION()
		virtual void OnRep_DMG_Water_Multiplier(const FGameplayAttributeData& OldDMG_Water_Multiplier);

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_DMG_Poison_Multiplier)
		FGameplayAttributeData DMG_Poison_Multiplier;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, DMG_Poison_Multiplier)
		UFUNCTION()
		virtual void OnRep_DMG_Poison_Multiplier(const FGameplayAttributeData& OldDMG_Poison_Multiplier);

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_DMG_Holy_Multiplier)
		FGameplayAttributeData DMG_Holy_Multiplier;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, DMG_Holy_Multiplier)
		UFUNCTION()
		virtual void OnRep_DMG_Holy_Multiplier(const FGameplayAttributeData& OldDMG_Holy_Multiplier);

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_DMG_Shadow_Multiplier)
		FGameplayAttributeData DMG_Shadow_Multiplier;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, DMG_Shadow_Multiplier)
		UFUNCTION()
		virtual void OnRep_DMG_Shadow_Multiplier(const FGameplayAttributeData& OldDMG_Shadow_Multiplier);

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_DMG_Ghost_Multiplier)
		FGameplayAttributeData DMG_Ghost_Multiplier;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, DMG_Ghost_Multiplier)
		UFUNCTION()
		virtual void OnRep_DMG_Ghost_Multiplier(const FGameplayAttributeData& OldDMG_Ghost_Multiplier);

		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_DMG_Undead_Multiplier)
		FGameplayAttributeData DMG_Undead_Multiplier;
	ATTRIBUTE_ACCESSORS(URPG_BaseAttributeSet, DMG_Undead_Multiplier)
		UFUNCTION()
		virtual void OnRep_DMG_Undead_Multiplier(const FGameplayAttributeData& OldDMG_Undead_Multiplier);


	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	UFUNCTION()
		virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);


	//funções ajuste de valores maximos
	UFUNCTION()
		void CalculateMana(ARPG_BaseCharacter* BaseCharacter);

	UFUNCTION()
		virtual void OnRep_ExtraMoveSpeed(const FGameplayAttributeData& OldExtraMoveSpeed);
	UFUNCTION()
		virtual void OnRep_MoveSpeedMultiplier(const FGameplayAttributeData& OldMoveSpeedMultiplier);
	UFUNCTION()
		virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
	UFUNCTION()
		virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
	UFUNCTION()
		virtual void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate);
	UFUNCTION()
		virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
		virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);
	UFUNCTION()
		virtual void OnRep_ManaRegenRate(const FGameplayAttributeData& OldManaRegenRate);

	UFUNCTION()
		virtual void OnRep_Oxygen(const FGameplayAttributeData& OldOxygen);
	UFUNCTION()
		virtual void OnRep_MaxOxygen(const FGameplayAttributeData& OldMaxOxygen);
	UFUNCTION()
		virtual void OnRep_OxygenRegenRate(const FGameplayAttributeData& OldOxygenRegenRate);


	UFUNCTION()
		virtual void OnRep_Food(const FGameplayAttributeData& OldHunger);
	UFUNCTION()
		virtual void OnRep_MaxFood(const FGameplayAttributeData& OldMaxHunger);
	UFUNCTION()
		virtual void OnRep_FoodRegenRate(const FGameplayAttributeData& OldHungerRegenRate);

	UFUNCTION()
		virtual void OnRep_Thirst(const FGameplayAttributeData& OldThirsty);
	UFUNCTION()
		virtual void OnRep_MaxThirst(const FGameplayAttributeData& OldMaxThirsty);
	UFUNCTION()
		virtual void OnRep_ThirstRegenRate(const FGameplayAttributeData& OldThirstyRegenRate);

	 
	 
};
