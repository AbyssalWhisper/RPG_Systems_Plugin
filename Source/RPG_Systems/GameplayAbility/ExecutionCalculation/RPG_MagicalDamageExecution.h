// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RPG_Systems/GameplayAbility/RPG_DamageTypeEnum.h"
#include "RPG_MagicalDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_MagicalDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:

	URPG_MagicalDamageExecution();
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<E_RPG_MagicalDamage> MagicalDamageType = E_RPG_MagicalDamage::Fire;

		FProperty* HealthProperty;

		void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;
};
