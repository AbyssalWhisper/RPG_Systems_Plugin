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
	
	// Try to activate the ability when an avatar is assigned.
	OnAbilityOnGranted,
	
	None
};


class ARPG_BaseCharacter;
UCLASS()
class RPG_SYSTEMS_API URPG_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	URPG_GameplayAbility();

	

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category = "Input")
	ERPG_AbilityActivationPolicy ActivationPolicy = ERPG_AbilityActivationPolicy::None;

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

#pragma region Input

	// Called to bind Input Pressed and Input Released events to the Avatar Actor's Enhanced Input Component if it is reachable. 
	void SetupEnhancedInputBindingsByInputAction(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	UInputAction* FindInputActionFromSettings(const FGameplayAbilitySpec& Spec);

	ETriggerEvent GetInputPressedTriggerType(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);
	ETriggerEvent GetInputReleasedTriggerType(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	
	
	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnInputStarted();
	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnInputTriggered();
	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnInputOngoing();
	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnInputCanceled();
	UFUNCTION(BlueprintNativeEvent, Category = "Input")
	void OnInputCompleted();
	

#pragma endregion
	// Override "OnRemoveAbility" to clean up Enhanced Input Bindings.
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;


	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
		ARPG_BaseCharacter* OwnerCharacter;
		virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
