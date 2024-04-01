// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RPG_Systems/BlueprintLibrary/RPG_BP_Library_Utilities.h"
#include "RPG_GameplayAbility.generated.h"

/**
 * 
 */
class ARPG_BaseCharacter;
UCLASS()
class RPG_SYSTEMS_API URPG_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
		ARPG_BaseCharacter* OwnerCharacter;
		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
			ERPG_AbilityInputID AbilityInput = ERPG_AbilityInputID::None;
		virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
