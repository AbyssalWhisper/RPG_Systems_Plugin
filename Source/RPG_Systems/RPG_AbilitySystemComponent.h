// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPG_AbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(blueprintCallable, Category = "Ability")
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void ProcessAbilityInput();

	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;
};
