// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverFunctionLibrary.h"
#include "MoveLibrary/MovementUtils.h"
#include <BetterUtilitiesBPLibrary.h>

#include "Chaos/Character/CharacterGroundConstraint.h"
#include "Chaos/Character/CharacterGroundConstraintContainer.h"
#include "Chaos/PhysicsObject.h"
#include "Chaos/PhysicsObjectInternalInterface.h"

#include "Chaos/DebugDrawQueue.h"

void UMoverFunctionLibrary::SetTransforms_WorldSpace(FMoverTickEndData& OutputState, FVector WorldLocation,
	FRotator WorldOrient, FVector WorldVelocity, UPrimitiveComponent* Base, FName BaseBone)
{
	FMoverDefaultSyncState& OutputSyncState = OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();
	OutputSyncState.SetTransforms_WorldSpace( WorldLocation,WorldOrient,WorldVelocity);
}

 

void UMoverFunctionLibrary::ChaosDrawDebugCapsuleRay(FVector Start, FVector End, float HalfHeight, float Radius, FRotator Rotation, FLinearColor DrawColor, bool bPersistentLines, float DrawTime, uint8 DepthPriority, float Thickness)
{
	UMoverFunctionLibrary::ChaosDrawDebugCapsule(Start, HalfHeight, Radius, Rotation, DrawColor.ToFColor(true),bPersistentLines, DrawTime,DepthPriority,Thickness);
	UMoverFunctionLibrary::ChaosDrawDebugLine(Start, End,DrawTime, DrawColor.ToFColor(true), bPersistentLines, DepthPriority,Thickness);
	UMoverFunctionLibrary::ChaosDrawDebugCapsule(End, HalfHeight, Radius, Rotation, DrawColor.ToFColor(true), bPersistentLines, DrawTime, DepthPriority, Thickness);

}

void UMoverFunctionLibrary::ChaosDrawDebugSphereRay(FVector Start, FVector End, float Radius, int32 Segments, FLinearColor DrawColor, bool bPersistentLines, float DrawTime, uint8 DepthPriority, float Thickness)
{
	UMoverFunctionLibrary::ChaosDrawDebugSphere(Start, Radius, Segments, DrawColor.ToFColor(true), bPersistentLines, DrawTime, DepthPriority, Thickness);
	UMoverFunctionLibrary::ChaosDrawDebugLine(Start, End, DrawTime, DrawColor.ToFColor(true), bPersistentLines, DepthPriority, Thickness);
	UMoverFunctionLibrary::ChaosDrawDebugSphere(End, Radius, Segments, DrawColor.ToFColor(true), bPersistentLines, DrawTime, DepthPriority, Thickness);

}

void UMoverFunctionLibrary::ChaosDrawDebugBoxRay(FVector Start, FVector End, FVector const& Extent, FRotator const& Rotation, FLinearColor DrawColor, bool bPersistentLines, float DrawTime, uint8 DepthPriority, float Thickness)
{
	UMoverFunctionLibrary::ChaosDrawDebugBox(Start, Extent, Rotation, DrawColor.ToFColor(true), bPersistentLines, DrawTime, DepthPriority, Thickness);
	UMoverFunctionLibrary::ChaosDrawDebugLine(Start, End, DrawTime, DrawColor.ToFColor(true), bPersistentLines, DepthPriority, Thickness);
    UMoverFunctionLibrary::ChaosDrawDebugBox(End, Extent, Rotation, DrawColor.ToFColor(true), bPersistentLines, DrawTime, DepthPriority, Thickness);

}
 

void UMoverFunctionLibrary::ChaosDrawDebugLine(FVector Start, FVector End, float DrawTime, FLinearColor DrawColor, bool bPersistentLines, uint8 DepthPriority, float Thickness)
{
#if WITH_EDITOR
	Chaos::FDebugDrawQueue::GetInstance().DrawDebugLine(Start, End, DrawColor.ToFColor(true), bPersistentLines, DrawTime, DepthPriority, Thickness);

#endif

	
}

void UMoverFunctionLibrary::ChaosDrawDebugSphere(FVector const& Center, float Radius, int32 Segments, FLinearColor DrawColor, bool bPersistentLines, float DrawTime, uint8 DepthPriority, float Thickness)
{
#if WITH_EDITOR
	Chaos::FDebugDrawQueue::GetInstance().DrawDebugSphere(Center, Radius, Segments, DrawColor.ToFColor(true), bPersistentLines, DrawTime, DepthPriority, Thickness);

#endif
	
}

void UMoverFunctionLibrary::ChaosDrawDebugCapsule(FVector const& Center, float HalfHeight, float Radius, FRotator Rotation, FLinearColor DrawColor, bool bPersistentLines, float DrawTime, uint8 DepthPriority, float Thickness)
{
#if WITH_EDITOR
	Chaos::FDebugDrawQueue::GetInstance().DrawDebugCapsule(Center, HalfHeight, Radius, Rotation.Quaternion(), DrawColor.ToFColor(true), bPersistentLines, DrawTime, DepthPriority, Thickness);

#endif
}

void UMoverFunctionLibrary::ChaosDrawDebugBox(FVector const& Center, FVector const& Extent, FRotator const& Rotation, FLinearColor DrawColor, bool bPersistentLines, float DrawTime, uint8 DepthPriority, float Thickness)
{
#if WITH_EDITOR
	// Converte a cor de FLinearColor para FColor
	FColor DebugColor = DrawColor.ToFColor(true);

	// Converte FRotator para FQuat
	FQuat RotationQuat = FQuat(Rotation);

	// Chama a função para desenhar a caixa no mundo usando Chaos Debug
	Chaos::FDebugDrawQueue::GetInstance().DrawDebugBox(Center, Extent, RotationQuat, DebugColor, bPersistentLines, DrawTime, DepthPriority, Thickness);
#endif
}

void UMoverFunctionLibrary::ChaosDrawDebugPoint(FVector const& Position, FLinearColor DrawColor, bool bPersistentLines, float DrawTime, uint8 DepthPriority, float Thickness)
{
#if WITH_EDITOR
	// Converte a cor de FLinearColor para FColor
	FColor DebugColor = DrawColor.ToFColor(true);

	// Chama a função para desenhar o ponto no mundo usando Chaos Debug
	Chaos::FDebugDrawQueue::GetInstance().DrawDebugPoint(Position, DebugColor, bPersistentLines, DrawTime, DepthPriority, Thickness);
#endif
}
