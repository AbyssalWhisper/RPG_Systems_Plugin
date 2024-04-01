// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Systems/GameplayAbility/RPG_BaseAttributeSet.h"
#include "RPG_PlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_PlayerAttributeSet : public URPG_BaseAttributeSet
{
	public:
	GENERATED_BODY()
		URPG_PlayerAttributeSet();
 

	
		virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


};
