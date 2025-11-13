// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosMover/Character/ChaosCharacterMoverComponent.h"
#include "CharacterVariants/MoverExamplesPhysicsCharacterMoverComponent.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "RPG_Systems/Mover/RPG_BaseMoverCharacter.h"
#include "Mover/Public/PhysicsMover/PhysicsCharacterMoverComponent.h"
#include "RPG_Systems/Mover/BaseMovementMode/RPG_BaseMovementMode.h"

#include "RPG_CharacterMoverComponent.generated.h"


class UPhysicsCharacterMovementModeInterface;
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType,meta=(BlueprintSpawnableComponent))
class RPG_SYSTEMS_API URPG_CharacterMoverComponent : public UChaosCharacterMoverComponent
{
protected:
	virtual void
	OnMoverPreSimulationTick(const FMoverTimeStep& TimeStep, const FMoverInputCmdContext& InputCmd) override;

private:
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


	/** Applies forces to physics objects on impact */
	virtual void OnHandleImpact(const FMoverOnImpactParams& ImpactParams) override;

	UFUNCTION(BlueprintPure, Category = Mover)
	virtual bool IsClimbing() const;

protected:

public:
#pragma region InstantMovementEffects
	
#pragma endregion 

};

class UCapsuleComponent;
class UCharacterMoverComponent;


/**
 * Stances: Applies settings to the actor to make them go into different stances like crouch or prone(not implemented), affects actor maxacceleration and capsule height
 * Note: This modifier currently uses the CDO of the actor to reset values to "standing" values.
 *		 This modifier also assumes the actor is using a capsule as it's updated component for now
 */
USTRUCT(BlueprintType)
struct FCapsuleSizeModifier : public FMovementModifierBase
{
	GENERATED_BODY()
public:
	FCapsuleSizeModifier();
	virtual ~FCapsuleSizeModifier() override {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mover", meta=(ClampMin=0))
	float HalfHeight;
	
	virtual bool HasGameplayTag(FGameplayTag TagToFind, bool bExactMatch) const override;
	
	/** Fired when this modifier is activated. */
	virtual void OnStart(UMoverComponent* MoverComp, const FMoverTimeStep& TimeStep, const FMoverSyncState& SyncState, const FMoverAuxStateContext& AuxState) override;
	
	/** Fired when this modifier is deactivated. */
	virtual void OnEnd(UMoverComponent* MoverComp, const FMoverTimeStep& TimeStep, const FMoverSyncState& SyncState, const FMoverAuxStateContext& AuxState) override;
	
	static void OnPostSimulationTick(UMoverComponent* MoverComp, UPrimitiveComponent* UpdatedPrimitive);

	// @return newly allocated copy of this FMovementModifier. Must be overridden by child classes
	virtual FMovementModifierBase* Clone() const override;

	virtual void NetSerialize(FArchive& Ar) override;

	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FString ToSimpleString() const override;

	virtual void AddReferencedObjects(class FReferenceCollector& Collector) override;

	virtual bool CanExpand_Internal(UMoverComponent* MoverComponent, USceneComponent* UpdatedComp, const FMoverSyncState& SyncState) const;
	
	// Whether expanding should be from the base of the capsule or not
	virtual bool ShouldExpandingMaintainBase(const UCharacterMoverComponent* MoverComp) const;

protected:
	// Modifies the updated component casted to a capsule component
	virtual void AdjustCapsule(UMoverComponent* MoverComp, float OldHalfHeight, float NewHalfHeight, float NewEyeHeight);

	// Applies any movement settings like acceleration or max speed changes
	void ApplyMovementSettings(UMoverComponent* MoverComp);
	
	// Reverts any movement settings like acceleration or max speed changes
	void RevertMovementSettings(UMoverComponent* MoverComp);

	
	void UpdateTargetHeight(UMoverComponent* MoverComp, float OriginalHalfHeight, float CrouchedHalfHeight, bool bCrouching)
	{
		//update the target height of all MovementModes
		for (auto Element : MoverComp->MovementModes)
		{	
			if (auto MovementMode = Element.Value)
			{
				if (MovementMode->GetClass()->ImplementsInterface(UPhysicsCharacterMovementModeInterface::StaticClass()))
				{
					// cast to uphysics character movement mode interface
					
					IPhysicsCharacterMovementModeInterface* Interactable = Cast<IPhysicsCharacterMovementModeInterface>(MovementMode);
					if (Interactable)
					{
						const float OriginalTargetHeight = Interactable->GetTargetHeight();
						const float Offset = OriginalTargetHeight - OriginalHalfHeight;
					
						Interactable->SetTargetHeightOverride(Offset + (bCrouching ? CrouchedHalfHeight : OriginalHalfHeight));
						
					}

					
				}
			}
		}
	}
};

template<>
struct TStructOpsTypeTraits<FCapsuleSizeModifier> : public TStructOpsTypeTraitsBase2<FCapsuleSizeModifier>
{
	enum
	{
		WithCopy = true
	};
};

