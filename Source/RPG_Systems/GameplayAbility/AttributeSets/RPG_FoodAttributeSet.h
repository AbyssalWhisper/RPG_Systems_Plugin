// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "RPG_FoodAttributeSet.generated.h"

/**
 * 
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class RPG_SYSTEMS_API URPG_FoodAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	public:
	URPG_FoodAttributeSet();
	
	UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_Food)
	FGameplayAttributeData 	Food;
	ATTRIBUTE_ACCESSORS(URPG_FoodAttributeSet, Food)
		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_MaxFood)
	FGameplayAttributeData MaxFood;
	ATTRIBUTE_ACCESSORS(URPG_FoodAttributeSet, MaxFood)
		UPROPERTY(BlueprintReadOnly, Category = "AttributeSet", ReplicatedUsing = OnRep_FoodRegenRate)
	FGameplayAttributeData FoodRegenRate;
	ATTRIBUTE_ACCESSORS(URPG_FoodAttributeSet, FoodRegenRate)

	UFUNCTION()
	virtual void OnRep_Food(const FGameplayAttributeData& OldHunger);
	UFUNCTION()
	virtual void OnRep_MaxFood(const FGameplayAttributeData& OldMaxHunger);
	UFUNCTION()
	virtual void OnRep_FoodRegenRate(const FGameplayAttributeData& OldHungerRegenRate);
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
