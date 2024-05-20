// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_UseItemConditionComponent.h"



bool URPG_UseItemConditionComponent::CanUse_Implementation(AActor* Owner)
{
	return true;
}

FText URPG_UseItemConditionComponent::GetCantUseMensage_Implementation()
{
	return FText();
}
