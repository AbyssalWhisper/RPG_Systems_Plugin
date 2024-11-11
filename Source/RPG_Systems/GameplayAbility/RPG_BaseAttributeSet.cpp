// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/GameplayAbility/RPG_BaseAttributeSet.h"
#include "Net/UnrealNetwork.h" 
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "RPG_Systems/Character/RPG_BaseCharacter.h"
#include "RPG_Systems/General_Interfaces/RPG_Attributes_Interface.h"

URPG_BaseAttributeSet::URPG_BaseAttributeSet() :
	
	Oxygen(100.f),
	MaxOxygen(100),
	OxygenRegenRate(1),
	Thirst(100.f),
	MaxThirst(100),
	ThirstRegenRate(1),
	ExtraMoveSpeed(0.f)
	, MoveSpeedMultiplier(1.f),
	Stamina(100),
	MaxStamina(100),
	StaminaRegenRate(1),
	MaxMana(20),
	Mana(20),
	ManaRegenRate(1),
	Attack(10),
	Armor(10),
	MagicArmor(10),
	DMG_Fire_Multiplier(1),
	DMG_Earth_Multiplier(1),
	DMG_Wind_Multiplier(1),
	DMG_Water_Multiplier(1),
	DMG_Poison_Multiplier(1),
	DMG_Holy_Multiplier(1),
	DMG_Shadow_Multiplier(1),
	DMG_Ghost_Multiplier(1),
	DMG_Undead_Multiplier(1)
{
}

 

void URPG_BaseAttributeSet::OnRep_MagicArmor(const FGameplayAttributeData& OldMagicArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, MagicArmor, OldMagicArmor);
}

void URPG_BaseAttributeSet::OnRep_DMG_Fire_Multiplier(const FGameplayAttributeData& OldDMG_Fire_Multiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, DMG_Fire_Multiplier, OldDMG_Fire_Multiplier);
}

void URPG_BaseAttributeSet::OnRep_DMG_Earth_Multiplier(const FGameplayAttributeData& OldDMG_Earth_Multiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, DMG_Earth_Multiplier, OldDMG_Earth_Multiplier);
}

void URPG_BaseAttributeSet::OnRep_DMG_Wind_Multiplier(const FGameplayAttributeData& OldDMG_Wind_Multiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, DMG_Wind_Multiplier, OldDMG_Wind_Multiplier);
}

void URPG_BaseAttributeSet::OnRep_DMG_Water_Multiplier(const FGameplayAttributeData& OldDMG_Water_Multiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, DMG_Water_Multiplier, OldDMG_Water_Multiplier);
}

void URPG_BaseAttributeSet::OnRep_DMG_Poison_Multiplier(const FGameplayAttributeData& OldDMG_Poison_Multiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, DMG_Poison_Multiplier, OldDMG_Poison_Multiplier);
}

void URPG_BaseAttributeSet::OnRep_DMG_Holy_Multiplier(const FGameplayAttributeData& OldDMG_Holy_Multiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, DMG_Holy_Multiplier, OldDMG_Holy_Multiplier);
}

void URPG_BaseAttributeSet::OnRep_DMG_Shadow_Multiplier(const FGameplayAttributeData& OldDMG_Shadow_Multiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, DMG_Shadow_Multiplier, OldDMG_Shadow_Multiplier);
}

void URPG_BaseAttributeSet::OnRep_DMG_Ghost_Multiplier(const FGameplayAttributeData& OldDMG_Ghost_Multiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, DMG_Ghost_Multiplier, OldDMG_Ghost_Multiplier);
}

void URPG_BaseAttributeSet::OnRep_DMG_Undead_Multiplier(const FGameplayAttributeData& OldDMG_Undead_Multiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, DMG_Undead_Multiplier, OldDMG_Undead_Multiplier);
}

void URPG_BaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	ARPG_BaseCharacter* BaseCharacter = Cast<ARPG_BaseCharacter>(GetOwningActor());
	AActor* Owner = Cast<AActor>(GetOwningActor());



	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(URPG_BaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(URPG_BaseAttributeSet, Stamina)))
	{
		if (BaseCharacter)
		{
			if (GetStamina() == GetMaxStamina())
			{ 
				BaseCharacter->AddGameplayTag(BaseCharacter->FullStaminaTag);
			}
			else
			{
				BaseCharacter->RemoveGameplayTag(BaseCharacter->FullStaminaTag);
			} 
		} 
		return;
	}
	
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(URPG_BaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(URPG_BaseAttributeSet, Thirst)))
	{
		if (BaseCharacter)
		{
			if (GetThirst() == GetMaxThirst())
			{ 
				BaseCharacter->AddGameplayTag(BaseCharacter->FullThirstTag);
			}
			else
			{
				BaseCharacter->RemoveGameplayTag(BaseCharacter->FullThirstTag);
			} 
		} 
		return;
	}
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(URPG_BaseAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(URPG_BaseAttributeSet, Oxygen)))
	{
		if (BaseCharacter)
		{
			if (GetOxygen() == GetMaxOxygen())
			{ 
				BaseCharacter->AddGameplayTag(BaseCharacter->FullOxygenTag);
			}
			else
			{
				BaseCharacter->RemoveGameplayTag(BaseCharacter->FullOxygenTag);
			}  
		} 
		return;
	}

}

void URPG_BaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeBaseChange(Attribute, NewValue); 

	
	if (GetStaminaAttribute() == Attribute)
	{
		CalculateStamina(NewValue);
		return;
	}
	
	if (GetThirstAttribute() == Attribute)
	{
		CalculateThirst(NewValue);
		return;
	}
	if (GetOxygenAttribute() == Attribute)
	{
		CalculateOxygen(NewValue);
		return;
	}
}



void URPG_BaseAttributeSet::CalculateStamina(float& StaminaValue)
{
	if (StaminaValue >= GetMaxStamina())
	{
		StaminaValue = GetMaxStamina();
	}
	if (StaminaValue <= 0)
	{
		StaminaValue = 0;
		if (GetOwningActor()->GetClass()->ImplementsInterface(URPG_Attributes_Interface::StaticClass()))
		{
			 
			IRPG_Attributes_Interface::Execute_OnStaminaEmpty(GetOwningActor());
		}
	}

	if (GetOwningActor()->GetClass()->ImplementsInterface(URPG_Attributes_Interface::StaticClass()))
	{
		 
		IRPG_Attributes_Interface::Execute_OnStaminaChanged(GetOwningActor(), GetStamina());
	}
}


void URPG_BaseAttributeSet::CalculateThirst(float& ThirstValue)
{
	if (ThirstValue >= GetMaxThirst())
	{
		ThirstValue = GetMaxThirst();
	}
	if (ThirstValue <= 0)
	{
		ThirstValue = 0;
		if (GetOwningActor()->GetClass()->ImplementsInterface(URPG_Attributes_Interface::StaticClass()))
		{
			//Server_Interact(InteractTarget);
			IRPG_Attributes_Interface::Execute_OnThirstEmpty(GetOwningActor());
		}
	}

	if (GetOwningActor()->GetClass()->ImplementsInterface(URPG_Attributes_Interface::StaticClass()))
	{
		//Server_Interact(InteractTarget);
		IRPG_Attributes_Interface::Execute_OnThirstChanged(GetOwningActor(), GetThirst());
	}
}

void URPG_BaseAttributeSet::CalculateOxygen(float& OxygenValue)
{
	if (OxygenValue >= GetMaxOxygen())
	{
		OxygenValue = GetMaxOxygen();
	}
	if (OxygenValue <= 0)
	{
		OxygenValue = 0;
		if (GetOwningActor()->GetClass()->ImplementsInterface(URPG_Attributes_Interface::StaticClass()))
		{ 
			IRPG_Attributes_Interface::Execute_OnOxygenEmpty(GetOwningActor());
		}
	}

	if (GetOwningActor()->GetClass()->ImplementsInterface(URPG_Attributes_Interface::StaticClass()))
	{ 
		IRPG_Attributes_Interface::Execute_OnOxygenChanged(GetOwningActor(), GetOxygen());
	}
}

void URPG_BaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, Oxygen, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, MaxOxygen, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, OxygenRegenRate, COND_None, REPNOTIFY_OnChanged);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, Thirst, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, MaxThirst, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, ThirstRegenRate, COND_None, REPNOTIFY_OnChanged);

	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, Attack, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, Armor, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, MagicArmor, COND_None, REPNOTIFY_OnChanged);

	 
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, DMG_Fire_Multiplier, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, DMG_Earth_Multiplier, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, DMG_Wind_Multiplier, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, DMG_Water_Multiplier, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, DMG_Poison_Multiplier, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, DMG_Holy_Multiplier, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, DMG_Shadow_Multiplier, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, DMG_Ghost_Multiplier, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, DMG_Undead_Multiplier, COND_None, REPNOTIFY_OnChanged);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, ExtraMoveSpeed, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, MoveSpeedMultiplier, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, Stamina, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, MaxStamina, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, Mana, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, StaminaRegenRate, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, MaxMana, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(URPG_BaseAttributeSet, ManaRegenRate, COND_None, REPNOTIFY_OnChanged);

}



void URPG_BaseAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, Armor, OldArmor);
}



void URPG_BaseAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldAttack)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, Attack, OldAttack);
}

void URPG_BaseAttributeSet::CalculateMana(ARPG_BaseCharacter* BaseCharacter)
{
	if (BaseCharacter)
	{
		if (GetMana() > GetMaxMana())
		{
			SetMana(GetMaxMana());
			BaseCharacter->AddGameplayTag(BaseCharacter->FullManaTag);
		}
		else
		{
			BaseCharacter->RemoveGameplayTag(BaseCharacter->FullManaTag);
		}

		if (GetMana() < 0)
		{
			SetMana(0);
			if (BaseCharacter)
			{
				BaseCharacter->OnManaEmpty();
			}
		}
		if (BaseCharacter)
		{
			BaseCharacter->OnManaChanged(GetMana());
		}
	}
	else
	{
		if (GetMana() > GetMaxMana())
		{
			SetMana(GetMaxMana());  
		} 

		if (GetMana() < 0)
		{
			SetMana(0); 
		} 
	} 
}

void URPG_BaseAttributeSet::OnRep_ExtraMoveSpeed(const FGameplayAttributeData& OldExtraMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, ExtraMoveSpeed, OldExtraMoveSpeed);
}

void URPG_BaseAttributeSet::OnRep_MoveSpeedMultiplier(const FGameplayAttributeData& OldMoveSpeedMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, MoveSpeedMultiplier, OldMoveSpeedMultiplier);
}

void URPG_BaseAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, Stamina, OldStamina);
}

void URPG_BaseAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, MaxStamina, OldMaxStamina);
}

void URPG_BaseAttributeSet::OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, StaminaRegenRate, OldStaminaRegenRate);

}

void URPG_BaseAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, Mana, OldMana);
}

void URPG_BaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, MaxMana, OldMaxMana);
}

void URPG_BaseAttributeSet::OnRep_ManaRegenRate(const FGameplayAttributeData& OldManaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, ManaRegenRate, OldManaRegenRate);
}

void URPG_BaseAttributeSet::OnRep_Oxygen(const FGameplayAttributeData& OldOxygen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, Oxygen, OldOxygen);
}

void URPG_BaseAttributeSet::OnRep_MaxOxygen(const FGameplayAttributeData& OldMaxOxygen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, MaxOxygen, OldMaxOxygen);
}

void URPG_BaseAttributeSet::OnRep_OxygenRegenRate(const FGameplayAttributeData& OldOxygenRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, OxygenRegenRate, OldOxygenRegenRate);
}



void URPG_BaseAttributeSet::OnRep_Thirst(const FGameplayAttributeData& OldThirst)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, Thirst, OldThirst);
}

void URPG_BaseAttributeSet::OnRep_MaxThirst(const FGameplayAttributeData& OldMaxThirst)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, MaxThirst, OldMaxThirst);
}

void URPG_BaseAttributeSet::OnRep_ThirstRegenRate(const FGameplayAttributeData& OldThirstRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPG_BaseAttributeSet, ThirstRegenRate, OldThirstRegenRate);
}
