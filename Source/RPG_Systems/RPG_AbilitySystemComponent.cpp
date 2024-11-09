// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_AbilitySystemComponent.h"

URPG_AbilitySystemComponent::URPG_AbilitySystemComponent()
{
	// Sets the Ability System Component to use "Mixed" replication mode. This will replicate minimal Gameplay Effects to Simulated Proxies and full info to everyone else.
	ReplicationMode = EGameplayEffectReplicationMode::Mixed;
	// Explicitly set the Ability System Component to replicate.
	SetIsReplicatedByDefault(true);
}

void URPG_AbilitySystemComponent::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			//if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			if (AbilitySpec.Ability && (AbilitySpec.Ability->AbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);

			}
		}
	}
}

void URPG_AbilitySystemComponent::ProcessAbilityInput()
{
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			UE_LOG(LogTemp, Warning, TEXT("Try use Ability: (%s)"), *GetPathNameSafe(this));

			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					/*
					const ULyraGameplayAbility* LyraAbilityCDO = CastChecked<ULyraGameplayAbility>(AbilitySpec->Ability);

					if (LyraAbilityCDO->GetActivationPolicy() == ELyraAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
					}*/
					AbilitiesToActivate.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActivate)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	AbilitiesToActivate.Empty();
	InputPressedSpecHandles.Empty();
}

void URPG_AbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	ProcessAbilityInput();
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

const UAttributeSet* URPG_AbilitySystemComponent::GetOrCreateAttributeSet(const TSubclassOf<UAttributeSet>& InAttributeSet)
{
	return GetOrCreateAttributeSubobject(InAttributeSet);
}

void URPG_AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URPG_AbilitySystemComponent::AddAttributeSet(TSubclassOf<UAttributeSet> AttributeClass)
{
	if (GetOwnerActor()->HasAuthority())
	{
		// Grant Attribute Sets if the array isn't empty.
		if (AttributeClass)
		{
			GetOrCreateAttributeSet(AttributeClass);
		}
	}
}

void URPG_AbilitySystemComponent::SetAttributeBaseValue(FGameplayAttribute Attribute, float BaseValue)
{
	if (HasAttributeSetForAttribute(Attribute))
	{
		SetNumericAttributeBase(Attribute, BaseValue);
	}
}
