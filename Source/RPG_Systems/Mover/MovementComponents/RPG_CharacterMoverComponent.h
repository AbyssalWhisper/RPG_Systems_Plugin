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
	
	/** Multiplies the impulse applied to physics objects on collisions to push them harder */
	UPROPERTY(EditAnywhere, Category = "Mover", meta=(ClampMin=0))
	float ImpactPhysicsForceMultiplier = 10.0f;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool TeleportImmediately(const FVector& Location, const FRotator& Orientation, const FVector& Velocity);
	
	virtual TObjectPtr<UBaseMovementMode> GetCurrentMovementMode();

	/** Applies forces to physics objects on impact */
	virtual void OnHandleImpact(const FMoverOnImpactParams& ImpactParams) override;

	UFUNCTION(BlueprintPure, Category = Mover)
	virtual bool IsClimbing() const;

#pragma region InstantMovementEffects
	
#pragma endregion 

};
