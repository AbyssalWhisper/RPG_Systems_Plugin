// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovementModeTransition.h"
#include "MoverTypes.h"
#include "RagdollTransition.generated.h"

/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URagdollTransition : public UBaseMovementModeTransition
{
	GENERATED_BODY()
	
	virtual FTransitionEvalResult OnEvaluate(const FSimulationTickParams& Params) const override;

protected:
	virtual void OnTrigger(const FSimulationTickParams& Params) override;
	float CanTransitionDelay = 3;
	float CurrentTransitionTime = 0;
	bool bCanToggleRagdoll = true;
	void CheckCanToggleRagdoll();
};

USTRUCT(BlueprintType)
struct RPG_SYSTEMS_API FRagdollAbilityInputs : public FMoverDataStructBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Mover)
	bool bWantsToRagdoll = false; 

	// @return newly allocated copy of this FMoverExampleAbilityInputs. Must be overridden by child classes
	virtual FMoverDataStructBase* Clone() const override
	{
		// TODO: ensure that this memory allocation jives with deletion method
		FRagdollAbilityInputs* CopyPtr = new FRagdollAbilityInputs(*this);
		return CopyPtr;
	}

	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override
	{
		Super::NetSerialize(Ar, Map, bOutSuccess);

		Ar.SerializeBits(&bWantsToRagdoll, 1);

		bOutSuccess = true;
		return true;
	}

	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }

	virtual void ToString(FAnsiStringBuilderBase& Out) const override
	{
		Super::ToString(Out);
		Out.Appendf("bWantsToRagdoll: %i\n", bWantsToRagdoll);
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override { Super::AddReferencedObjects(Collector); }
};
