// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/GameplayAbility/ExecutionCalculation/RPG_MagicalDamageExecution.h"
#include "RPG_Systems/GameplayAbility/RPG_BaseAttributeSet.h"

struct EffectsAttributes
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)
	EffectsAttributes()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_BaseAttributeSet, Armor, Target, true)
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_BaseAttributeSet, Health, Target, true)
	}

};

static EffectsAttributes& GetEffectsAttributes()
{
	static EffectsAttributes EffectStatsVar;
	return  EffectStatsVar;
}

URPG_MagicalDamageExecution::URPG_MagicalDamageExecution() {
	HealthProperty = FindFieldChecked<FProperty>(URPG_BaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(URPG_BaseAttributeSet, Health));
	RelevantAttributesToCapture.Add(GetEffectsAttributes().HealthDef);
//	RelevantAttributesToCapture.Add(GetEffectsAttributes().AttackDef);
	RelevantAttributesToCapture.Add(GetEffectsAttributes().ArmorDef);
}

void URPG_MagicalDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
}
