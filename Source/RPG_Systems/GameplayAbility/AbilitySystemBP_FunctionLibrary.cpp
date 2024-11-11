// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/GameplayAbility/AbilitySystemBP_FunctionLibrary.h"


void UAbilitySystemBP_FunctionLibrary::InitGameplayTagBlueprintPropertyMap(
	UObject* OwnerVariable, FBP_GameplayTagBlueprintPropertyMap& GameplayTagBlueprintPropertyMap, UAbilitySystemComponent* ASC)
{
	if (ASC && OwnerVariable)
	{
		GameplayTagBlueprintPropertyMap.GameplayTagPropertyMap.Initialize(OwnerVariable,ASC);
	}
}
