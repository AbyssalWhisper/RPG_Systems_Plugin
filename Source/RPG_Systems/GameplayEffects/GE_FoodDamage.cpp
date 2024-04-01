// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/GameplayEffects/GE_FoodDamage.h"

UGE_FoodDamage::UGE_FoodDamage() {
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Modifiers.SetNum(1);
	StackLimitCount = 1;
	StackingType = EGameplayEffectStackingType::AggregateBySource;
}