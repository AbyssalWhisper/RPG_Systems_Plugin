// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/GameplayAbility/ExecutionCalculation/RPG_PhysicalDamageExec.h"
#include "RPG_Systems/GameplayAbility/RPG_BaseAttributeSet.h"

struct  Damage
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	Damage()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_BaseAttributeSet, Health, Target, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_BaseAttributeSet, Attack, Source, false)
			DEFINE_ATTRIBUTE_CAPTUREDEF(URPG_BaseAttributeSet, Armor, Target, false)
	}

};

static Damage& GetDamageStats()
{
	static Damage DamageStaticVar;
	return  DamageStaticVar;
}

URPG_PhysicalDamageExec::URPG_PhysicalDamageExec()
{
	HealthProperty = FindFieldChecked<FProperty>(URPG_BaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(URPG_BaseAttributeSet, Health));
	RelevantAttributesToCapture.Add(GetDamageStats().HealthDef);
	RelevantAttributesToCapture.Add(GetDamageStats().AttackDef);
	RelevantAttributesToCapture.Add(GetDamageStats().ArmorDef);
}

void URPG_PhysicalDamageExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	 
	float Damage;

	float PercentValue = 0.f;
	PercentValue += Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("DamageMultiplier")), false, 1.f);

	//Ataque do player
	float AttackSource;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStats().AttackDef, FAggregatorEvaluateParameters(), AttackSource);
	 
	//Armadura do alvo
	float TargetArmor;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStats().ArmorDef, FAggregatorEvaluateParameters(), TargetArmor);


	Damage = AttackSource * (TargetArmor > 0 ? (AttackSource / (AttackSource + TargetArmor)) : 2 - (AttackSource / (AttackSource - TargetArmor)));

	Damage = Damage * PercentValue;
	/*
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, FString::SanitizeFloat(AttackSource));
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, FString::SanitizeFloat(TargetArmor));
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, FString::SanitizeFloat(Damage));
	//Damage
	if (ExecutionParams.GetSourceAbilitySystemComponent() && ExecutionParams.GetTargetAbilitySystemComponent())
	{
		if (ExecutionParams.GetTargetAbilitySystemComponent()->GetOwner())
		{

			ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(ExecutionParams.GetTargetAbilitySystemComponent()->GetOwner());

			if (BaseCharacter)
			{

				if (Damage > 0)
				{

					BaseCharacter->ReceivedDamage(ExecutionParams.GetSourceAbilitySystemComponent()->GetOwner(), -Damage);
				}

			}
		}

	}
	*/

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealthProperty, EGameplayModOp::Additive, -Damage));
}