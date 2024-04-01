// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/GameplayAbility/RPG_PlayerAttributeSet.h"
#include "Net/UnrealNetwork.h" 
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"

URPG_PlayerAttributeSet::URPG_PlayerAttributeSet()
{
}

void URPG_PlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

}

void URPG_PlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeBaseChange(Attribute,NewValue);
}
 

void URPG_PlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
  