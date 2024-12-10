// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "RPG_Systems/Mover/RPG_BaseMoverCharacter.h"
#include "RPG_CharacterMoverComponent.generated.h"


/**
 * 
 */
UCLASS(Blueprintable,BlueprintType,meta=(BlueprintSpawnableComponent))
class RPG_SYSTEMS_API URPG_CharacterMoverComponent : public UCharacterMoverComponent
{
	GENERATED_BODY()
	
public:

	URPG_CharacterMoverComponent();
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	ARPG_BaseMoverCharacter* Character;


	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual TObjectPtr<UBaseMovementMode> GetCurrentMovementMode();


	UFUNCTION(BlueprintCallable,BlueprintPure,DisplayName="HasGameplayTag", Category = Mover)
	bool BP_HasGameplayTag(FGameplayTag Tag, bool bExactMatch)const;

	UFUNCTION(BlueprintPure, Category = Mover)
	virtual bool IsClimbing() const;

};
