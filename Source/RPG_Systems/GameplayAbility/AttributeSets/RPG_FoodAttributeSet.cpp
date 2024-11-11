// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_FoodAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "RPG_Systems/Character/RPG_BaseCharacter.h"
#include "RPG_Systems/General_Interfaces/RPG_Attributes_Interface.h"

URPG_FoodAttributeSet::URPG_FoodAttributeSet() : Food(100.f),
                                                 MaxFood(100),
                                                 FoodRegenRate(1)
{
}

void URPG_FoodAttributeSet::OnRep_Food(const FGameplayAttributeData& OldFood)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_FoodAttributeSet, Food, OldFood);
}

void URPG_FoodAttributeSet::OnRep_MaxFood(const FGameplayAttributeData& OldMaxFood)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_FoodAttributeSet, MaxFood, OldMaxFood);
}

void URPG_FoodAttributeSet::OnRep_FoodRegenRate(const FGameplayAttributeData& OldFoodRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_FoodAttributeSet, FoodRegenRate, OldFoodRegenRate);
}

void URPG_FoodAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (GetFoodAttribute() == Attribute)
	{
		if (NewValue >= GetMaxFood())
		{
			NewValue = GetMaxFood();
		}
		if (NewValue <= 0)
		{
			NewValue = 0;
			if (GetOwningActor()->GetClass()->ImplementsInterface(URPG_Attributes_Interface::StaticClass()))
			{
			 
				IRPG_Attributes_Interface::Execute_OnFoodEmpty(GetOwningActor());
			}
		}

		if (GetOwningActor()->GetClass()->ImplementsInterface(URPG_Attributes_Interface::StaticClass()))
		{ 
			IRPG_Attributes_Interface::Execute_OnFoodChanged(GetOwningActor(), GetFood());
		}
		return;
	}
}



void URPG_FoodAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_FoodAttributeSet, Food, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_FoodAttributeSet, MaxFood, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_FoodAttributeSet, FoodRegenRate, COND_None, REPNOTIFY_OnChanged);
}

void URPG_FoodAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(URPG_FoodAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(URPG_FoodAttributeSet, Food)))
	{
		ARPG_BaseCharacter* BaseCharacter = Cast<ARPG_BaseCharacter>(GetOwningActor());
		if (BaseCharacter)
		{
			if (GetFood() == GetMaxFood())
			{ 
				BaseCharacter->AddGameplayTag(BaseCharacter->FullFoodTag);
			}
			else
			{
				BaseCharacter->RemoveGameplayTag(BaseCharacter->FullFoodTag);
			} 
		} 
		return;
	}
}
