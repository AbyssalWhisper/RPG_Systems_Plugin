// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/GameplayAbility/RPG_GameplayAbility.h"

#include "EnhancedInputComponent.h"
#include "RPG_Systems/Character/RPG_BaseCharacter.h"

void URPG_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

URPG_GameplayAbility::URPG_GameplayAbility()
{
	// Sets the ability to default to Instanced Per Actor.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPG_GameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	// Set the "Avatar Character" reference.
	AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor);

	switch (ActivationPolicy) {
	case ERPG_AbilityActivationPolicy::OnInputTriggered:
		// Set up Bindings for Enhanced Input.
		SetupEnhancedInputBindings(ActorInfo, Spec);
		break;
	case ERPG_AbilityActivationPolicy::OnAbilityOnGranted:
		// Try to Activate immediately if "Activate Ability On Granted" is true.
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		break;
	}
}

void URPG_GameplayAbility::SetupEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (IsValid(ActorInfo->AvatarActor.Get()))
	{
		if (const APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
		{
			if (const AController* PawnController = AvatarPawn->GetController())
			{
				if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PawnController->InputComponent.Get()))
				{
					if (URPG_GameplayAbility* AbilityInstance = Cast<URPG_GameplayAbility>(Spec.Ability.Get()))
					{
						// Check to see if the "Activation Input Action" is valid.
						if (IsValid(AbilityInstance->ActivationInputAction))
						{
							// If we have a valid "Input Pressed Trigger" type bind the pressed event.
							if (InputPressedTriggerType != ETriggerEvent::None)
							{
								EnhancedInputComponent->BindAction(AbilityInstance->ActivationInputAction, AbilityInstance->InputPressedTriggerType, AbilityInstance, &ThisClass::HandleInputPressedEvent, ActorInfo, Spec.Handle);
							}

							// If we have a valid "Input Released Trigger" type bind the released event.
							if (InputReleasedTriggerType != ETriggerEvent::None)
							{
								EnhancedInputComponent->BindAction(AbilityInstance->ActivationInputAction, AbilityInstance->InputReleasedTriggerType, AbilityInstance, &ThisClass::HandleInputReleasedEvent, ActorInfo, Spec.Handle);
							}
						}
					}
				}
			}
		}
	}
}

void URPG_GameplayAbility::HandleInputPressedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle)
{
	// Find the Ability Spec based on the passed in information and set a reference.
	if (FGameplayAbilitySpec* Spec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(SpecHandle))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			Spec->InputPressed = true;
			
			if (Spec->IsActive())
			{
				if (Spec->Ability.Get()->bReplicateInputDirectly && AbilitySystemComponent->IsOwnerActorAuthoritative() == false)
				{
					AbilitySystemComponent->ServerSetInputPressed(Spec->Ability.Get()->GetCurrentAbilitySpecHandle());
				}

				AbilitySystemComponent->AbilitySpecInputPressed(*Spec);

				// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
				AbilitySystemComponent->InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec->Handle, Spec->ActivationInfo.GetActivationPredictionKey());
			}
			else
			{
				// Ability is not active, so try to activate it
				AbilitySystemComponent->TryActivateAbility(SpecHandle);
			}
		}
	}
}

void URPG_GameplayAbility::HandleInputReleasedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle)
{
	// Find the Ability Spec based on the passed in information and set a reference.
	if (FGameplayAbilitySpec* Spec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(SpecHandle))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get())
		{
			Spec->InputPressed = false;
			
			if (Spec && Spec->IsActive())
			{
				if (Spec->Ability.Get()->bReplicateInputDirectly && AbilitySystemComponent->IsOwnerActorAuthoritative() == false)
				{
					AbilitySystemComponent->ServerSetInputReleased(SpecHandle);
				}

				AbilitySystemComponent->AbilitySpecInputReleased(*Spec);

				// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
				AbilitySystemComponent->InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, SpecHandle, Spec->ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void URPG_GameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (IsValid(ActorInfo->AvatarActor.Get()))
	{
		if (const APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
		{
			if (const AController* PawnController = AvatarPawn->GetController())
			{
				if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PawnController->InputComponent.Get()))
				{
					// We need to clear the bindings from the Enhanced Input Component when the Ability is removed.
					EnhancedInputComponent->ClearBindingsForObject(Spec.Ability.Get());
				}
			}
		}
	}
	
	Super::OnRemoveAbility(ActorInfo, Spec);
}