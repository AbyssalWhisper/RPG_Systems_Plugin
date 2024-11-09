// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_HealthAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "RPG_Systems/Character/RPG_BaseCharacter.h"

URPG_HealthAttributeSet::URPG_HealthAttributeSet() :
	Health(100.f),
	MaxHealth(100),
	HealthRegenRate(1)
{

	
}

void URPG_HealthAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void URPG_HealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	ARPG_BaseCharacter* BaseCharacter = Cast<ARPG_BaseCharacter>(GetOwningActor());
	AActor* Owner = Cast<AActor>(GetOwningActor());

	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(URPG_HealthAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(URPG_HealthAttributeSet, Health)))
	{
		if (BaseCharacter)
		{
			
			if (GetHealth() == GetMaxHealth())
			{
				BaseCharacter->AddGameplayTag(BaseCharacter->FullHealthTag);
				 
			}
			else
			{
				BaseCharacter->RemoveGameplayTag(BaseCharacter->FullHealthTag);
			}

		}
		return;
	}
}

void URPG_HealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URPG_HealthAttributeSet, Health, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_HealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_HealthAttributeSet,  HealthRegenRate, COND_None, REPNOTIFY_OnChanged);

}

void URPG_HealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (GetHealthAttribute() == Attribute)
	{
		if (NewValue >= GetMaxHealth())
		{
			NewValue = GetMaxHealth();
		}
		if (NewValue <= 0)
		{
			NewValue = 0;
			if (GetOwningActor()->GetClass()->ImplementsInterface(URPG_Attributes_Interface::StaticClass()))
			{
				//Server_Interact(InteractTarget);
				IRPG_Attributes_Interface::Execute_OnHealthEmpty(GetOwningActor());
			}
		}

		if (GetOwningActor()->GetClass()->ImplementsInterface(URPG_Attributes_Interface::StaticClass()))
		{
			//Server_Interact(InteractTarget);
			IRPG_Attributes_Interface::Execute_OnHealthChanged(GetOwningActor(), GetHealth());
			return;
		}
	}
}

void URPG_HealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_HealthAttributeSet, Health, OldHealth);
}

void URPG_HealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_HealthAttributeSet, MaxHealth, OldMaxHealth);
}

void URPG_HealthAttributeSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_HealthAttributeSet, HealthRegenRate, OldHealthRegenRate);
}
