// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/GameplayAbility/RPG_GameplayAbility.h"

#include "BetterUtilitiesBPLibrary.h"
#include "EnhancedInputComponent.h"
#include "RPG_Systems/Character/RPG_BaseCharacter.h"

#include "RPG_Systems/RPG_AbilitySystemComponent.h"
#include "RPG_Systems/RPG_SystemsDeveloperSettings.h"

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

	if (Spec.InputID>=0)
	{
		SetupEnhancedInputBindingsByInputAction(ActorInfo,Spec);
	}
	
	switch (ActivationPolicy)
	{
	case ERPG_AbilityActivationPolicy::None:

		break;
	case ERPG_AbilityActivationPolicy::OnAbilityOnGranted:
		// Try to Activate immediately if "Activate Ability On Granted" is true.
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		break;
	}
	
}


void URPG_GameplayAbility::SetupEnhancedInputBindingsByInputAction(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
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
						UInputAction* InputAction = FindInputActionFromSettings(Spec);
						// Check to see if the "Activation Input Action" is valid.
						if (InputAction)
						{

							EnhancedInputComponent->BindAction(InputAction,ETriggerEvent::Started, GetAbilitySystemComponentFromActorInfo(), &UAbilitySystemComponent::AbilityLocalInputPressed,Spec.InputID);
							EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed,GetAbilitySystemComponentFromActorInfo(), &UAbilitySystemComponent::AbilityLocalInputReleased,Spec.InputID);

							EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &URPG_GameplayAbility::OnInputStarted);
							EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &URPG_GameplayAbility::OnInputTriggered);
							EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Ongoing, this, &URPG_GameplayAbility::OnInputOngoing);
							EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &URPG_GameplayAbility::OnInputCanceled);
							EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &URPG_GameplayAbility::OnInputCompleted);
							
						}
					}
				}
			}
		}
	}
}



UInputAction* URPG_GameplayAbility::FindInputActionFromSettings(
	const FGameplayAbilitySpec& Spec)
{
	
    const URPG_SystemsDeveloperSettings* Settings = GetDefault<URPG_SystemsDeveloperSettings>();
    {
    	
    	if (Spec.InputID > (Settings->AbilitiesInputActions.Num() - 1))
    	{
    		UBetterUtilities::DebugLog(GetPathName()+" Input ID is out of range");
    		UBetterUtilities::DebugLog(GetPathName()+" Input ID: " + FString::FromInt(Spec.InputID));
    		return nullptr;
    	}
    	auto FoundAction = Settings->AbilitiesInputActions.Array()[Spec.InputID];
    	auto loadedAction = FoundAction.Get();
    	if (loadedAction)
    	{
    		UBetterUtilities::DebugLog(GetPathName()+" Found Input Action: " + loadedAction->GetFName().ToString());
    		return loadedAction;
    	}
    }
	
    return nullptr;
}

ETriggerEvent URPG_GameplayAbility::GetInputPressedTriggerType(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	auto handle =  GetAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromClass(GetClass());
	for (const FGameplayTag& Tag : handle->GetDynamicSpecSourceTags())
	{
		if (Tag == RPG_InputPressedTriggerTypeNone) return ETriggerEvent::None;
		if (Tag == RPG_InputPressedTriggerTypeTriggered) return ETriggerEvent::Triggered;
		if (Tag == RPG_InputPressedTriggerTypeStarted) return ETriggerEvent::Started;
		if (Tag == RPG_InputPressedTriggerTypeOngoing) return ETriggerEvent::Ongoing;
		if (Tag == RPG_InputPressedTriggerTypeCanceled) return ETriggerEvent::Canceled;
		if (Tag == RPG_InputPressedTriggerTypeCompleted) return ETriggerEvent::Completed;
	}
	return ETriggerEvent::None;
}

ETriggerEvent URPG_GameplayAbility::GetInputReleasedTriggerType(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	auto handle =  GetAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromClass(GetClass());
	for (const FGameplayTag& Tag : handle->GetDynamicSpecSourceTags())
	{
		if (Tag == RPG_InputReleasedTriggerTypeNone) return ETriggerEvent::None;
		if (Tag == RPG_InputReleasedTriggerTypeTriggered) return ETriggerEvent::Triggered;
		if (Tag == RPG_InputReleasedTriggerTypeStarted) return ETriggerEvent::Started;
		if (Tag == RPG_InputReleasedTriggerTypeOngoing) return ETriggerEvent::Ongoing;
		if (Tag == RPG_InputReleasedTriggerTypeCanceled) return ETriggerEvent::Canceled;
		if (Tag == RPG_InputReleasedTriggerTypeCompleted) return ETriggerEvent::Completed;
	}
	return ETriggerEvent::None;
}

void URPG_GameplayAbility::OnInputStarted_Implementation()
{
}

void URPG_GameplayAbility::OnInputTriggered_Implementation()
{
}

void URPG_GameplayAbility::OnInputOngoing_Implementation()
{
}

void URPG_GameplayAbility::OnInputCanceled_Implementation()
{
}

void URPG_GameplayAbility::OnInputCompleted_Implementation()
{
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
