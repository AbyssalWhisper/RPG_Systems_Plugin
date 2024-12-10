// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_BaseMovementModeTransition.h"

#include "MoverComponent.h"
#include "MoverSimulationTypes.h"
#include "TraceUtils.h"
#include "MoveLibrary/AirMovementUtils.h"
#include "RPG_Systems/Mover/MoverFunctionLibrary.h"

bool URPG_BaseMovementModeTransition::CapsuleTraceSingle(UObject* WorldContextObject, const FVector Start,
                                                         const FVector End, float Radius, float HalfHeight, const FRotator Orientation, ETraceTypeQuery TraceChannel,
                                                         bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit,
                                                         bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	return UTraceUtils::CapsuleTraceSingle(WorldContextObject, Start,End,Radius,HalfHeight,
		Orientation,TraceChannel,bTraceComplex,ActorsToIgnore,DrawDebugType,OutHit,bIgnoreSelf,TraceColor,TraceHitColor,DrawTime);
}

bool URPG_BaseMovementModeTransition::SphereTraceSingle(UObject* WorldContextObject, const FVector Start,
	const FVector End, float Radius, ETraceTypeQuery TraceChannel, bool bTraceComplex,
	const TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace::Type DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf,
	FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime)
{
	
	return UKismetSystemLibrary::SphereTraceSingle(WorldContextObject,
		Start,End,Radius,TraceChannel,bTraceComplex,ActorsToIgnore,
		DrawDebugType,OutHit,bIgnoreSelf,TraceColor,TraceHitColor
		,DrawTime);
	
}

FCharacterDefaultInputs& URPG_BaseMovementModeTransition::GetDefaultInputs(const FSimulationTickParams& Params) const
{
	return *Params.StartState.InputCmd.InputCollection.FindMutableDataByType<FCharacterDefaultInputs>();
}

bool URPG_BaseMovementModeTransition::HasInputTags(const FSimulationTickParams& Params, FGameplayTag InputTag) const
{
	const FRPG_MoverInputTags* inputtags = Params.StartState.InputCmd.InputCollection.FindDataByType<FRPG_MoverInputTags>();
	if(!inputtags)return false;
	return inputtags->Tags.HasTagExact(InputTag);
}

AActor* URPG_BaseMovementModeTransition::GetOwner() const
{
	return GetMoverComponent()->GetOwner();
}



