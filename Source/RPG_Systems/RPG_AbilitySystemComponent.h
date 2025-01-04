// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "InputAction.h"
#include "GameplayAbility/RPG_GameplayAbility.h"
#include "NativeGameplayTags.h"
#include "RPG_AbilitySystemComponent.generated.h"

/**
 * 
 */
// Gameplay tags

RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_OverrideInput);

RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputPressedTriggerTypeNone);
RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputPressedTriggerTypeTriggered);
RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputPressedTriggerTypeStarted);
RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputPressedTriggerTypeOngoing);
RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputPressedTriggerTypeCanceled);
RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputPressedTriggerTypeCompleted);

RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputReleasedTriggerTypeNone);
RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputReleasedTriggerTypeTriggered);
RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputReleasedTriggerTypeStarted);
RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputReleasedTriggerTypeOngoing);
RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputReleasedTriggerTypeCanceled);
RPG_SYSTEMS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RPG_InputReleasedTriggerTypeCompleted);

UCLASS(Blueprintable,meta=(BlueprintSpawnableComponent))
class RPG_SYSTEMS_API URPG_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	URPG_AbilitySystemComponent();
	/*
	UFUNCTION(blueprintCallable, Category = "Ability")
	void AbilityInputTagPressed(FGameplayTag InputTag);
	UFUNCTION(blueprintCallable, Category = "Ability")
	void AbilityInputTagReleased(FGameplayTag InputTag);
	*/
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;
	const UAttributeSet* GetOrCreateAttributeSet(const TSubclassOf<UAttributeSet>& InAttributeSet);
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Attributes")
	TArray<TSubclassOf<UAttributeSet>> AttributeSetsClass;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Attributes")
	TMap<FGameplayAttribute, float> AttributeBaseValues;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	void AddAttributeSet(TSubclassOf<UAttributeSet> AttributeClass);
	UFUNCTION(BlueprintCallable, Category = "Ability")

	void SetAttributeBaseValue(FGameplayAttribute Attribute, float BaseValue);
	
	//Use this function together with AbilityInputTagPressed
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void RemoveAbility(TSubclassOf<URPG_GameplayAbility> AbilityClass);
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void GiveAbilityWithInputAction(TSubclassOf<URPG_GameplayAbility> AbilityClass, UInputAction* Input);


};
