// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystemBP_FunctionLibrary.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct RPG_SYSTEMS_API FBP_GameplayTagBlueprintPropertyMap{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};


UCLASS()
class RPG_SYSTEMS_API UAbilitySystemBP_FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static void InitGameplayTagBlueprintPropertyMap(UObject* OwnerVariable,UPARAM(ref) FBP_GameplayTagBlueprintPropertyMap& GameplayTagBlueprintPropertyMap, UAbilitySystemComponent* ASC);
};

