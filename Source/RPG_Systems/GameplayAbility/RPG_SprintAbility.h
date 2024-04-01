// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Systems/GameplayAbility/RPG_GameplayAbility.h"
#include "RPG_SprintAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_SprintAbility : public URPG_GameplayAbility
{
	GENERATED_BODY()
public:
		virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
