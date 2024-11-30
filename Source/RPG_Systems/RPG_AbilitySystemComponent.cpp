// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_AbilitySystemComponent.h"

#include "BetterUtilitiesBPLibrary.h"


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
		for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				AbilitySpec.InputPressed = true;
				
				if (AbilitySpec.IsActive())
				{
					if (AbilitySpec.Ability.Get()->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
					{
						ServerSetInputPressed(AbilitySpec.Ability.Get()->GetCurrentAbilitySpecHandle());
					}

					AbilitySpecInputPressed(AbilitySpec);

					// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			
				}
				else
				{
					// Ability is not active, so try to activate it
					TryActivateAbility(AbilitySpec.Handle);
				}
				
			}
		}
	}
}

void URPG_AbilitySystemComponent::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (InputTag.IsValid())
	{
		for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				AbilitySpec.InputPressed = false;

				if (AbilitySpec.IsActive())
				{
					if (AbilitySpec.Ability.Get()->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
					{
						ServerSetInputReleased(AbilitySpec.Handle);
					}

					AbilitySpecInputReleased(AbilitySpec);

					// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
				}
			}
		}
	}
}


void URPG_AbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

const UAttributeSet* URPG_AbilitySystemComponent::GetOrCreateAttributeSet(const TSubclassOf<UAttributeSet>& InAttributeSet)
{
	return GetOrCreateAttributeSubobject(InAttributeSet);
}

void URPG_AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto Element : AttributeSetsClass)
	{
		AddAttributeSet(Element);
	}
	for (auto Element : AttributeBaseValues)
	{
		SetAttributeBaseValue(Element.Key,Element.Value);
	}
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

void URPG_AbilitySystemComponent::RemoveAbility(TSubclassOf<URPG_GameplayAbility> AbilityClass)
{
	if(!GetOwner()->HasAuthority())return;
	
	if (AbilityClass)
	{
		for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilityClass == AbilitySpec.Ability.GetClass())
			{
				ClearAbility(AbilitySpec.Handle);
				UBetterUtilities::DebugLog("Ability removed");
				break;
			}
		}
	}
}

void URPG_AbilitySystemComponent::GiveAbilityAndTagAbility(TSubclassOf<URPG_GameplayAbility> AbilityClass,
                                                           FGameplayTag GameplayTag)
{
	if (AbilityClass && GameplayTag.IsValid())
	{
		
		FGameplayAbilitySpecHandle AbilityHandle = GiveAbility(FGameplayAbilitySpec(AbilityClass, 1));
	
		auto abilityspec = FindAbilitySpecFromHandle(AbilityHandle);
		if (abilityspec)
		{
			abilityspec->DynamicAbilityTags.AddTag(GameplayTag);
		}
	}

	
}
