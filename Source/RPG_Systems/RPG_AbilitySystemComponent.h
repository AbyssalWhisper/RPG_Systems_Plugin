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
	URPG_AbilitySystemComponent();
	
	UFUNCTION(blueprintCallable, Category = "Ability")
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void ProcessAbilityInput();

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
	
};
