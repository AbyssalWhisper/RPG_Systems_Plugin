// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "Abilities/GameplayAbility.h"
#include "RPG_Systems/BlueprintLibrary/RPG_BP_Library_Utilities.h"
#include "RPG_GameplayAbility.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ERPG_AbilityActivationPolicy : uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,
	// Try to activate the ability when an avatar is assigned.
	OnAbilityOnGranted
};


class ARPG_BaseCharacter;
UCLASS()
class RPG_SYSTEMS_API URPG_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	URPG_GameplayAbility();

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = "Input")
	ERPG_AbilityActivationPolicy ActivationPolicy;

	// Which Input Action to bind the activation event to.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input",meta=(EditCondition="ActivationPolicy==ERPG_AbilityActivationPolicy::OnInputTriggered",EditConditionHides))
	UInputAction* ActivationInputAction = nullptr;

	// What Input Action trigger type to listen to for input pressed events.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input",meta=(EditCondition="ActivationPolicy==ERPG_AbilityActivationPolicy::OnInputTriggered",EditConditionHides))
	ETriggerEvent InputPressedTriggerType = ETriggerEvent::Started;

	// What Input Action trigger type to listen to for input released events.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input",meta=(EditCondition="ActivationPolicy==ERPG_AbilityActivationPolicy::OnInputTriggered",EditConditionHides))
	ETriggerEvent InputReleasedTriggerType = ETriggerEvent::Completed;

	// Returns the "Avatar Character" associated with this Gameplay Ability.
	// Will return null if the Avatar Actor does not derive from Character.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ACharacter* GetAvatarCharacter() const { return AvatarCharacter.Get(); }

protected:

	// Keep a pointer to "Avatar Character" so we don't have to cast to Character in instanced abilities owned by a Character derived class.
	TWeakObjectPtr<ACharacter> AvatarCharacter = nullptr;
	
	// Think of this as "BeginPlay".
	// Add logic here that should run when the Ability is first initialized.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	// Called to bind Input Pressed and Input Released events to the Avatar Actor's Enhanced Input Component if it is reachable. 
	void SetupEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	// Called when the "Activation Input Action" is triggered.
	void HandleInputPressedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle);

	// Called when the "Activation Input Action" is completed.
	void HandleInputReleasedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle);

	// Override "OnRemoveAbility" to clean up Enhanced Input Bindings.
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;


	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
		ARPG_BaseCharacter* OwnerCharacter;
		virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
