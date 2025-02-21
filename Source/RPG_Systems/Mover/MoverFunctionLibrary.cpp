// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverFunctionLibrary.h"
#include "MoveLibrary/MovementUtils.h"
#include <BetterUtilitiesBPLibrary.h>

#include "Chaos/PhysicsObject.h"

#include "Chaos/DebugDrawQueue.h"
#include "MoverComponent.h"


bool FRPG_MoverInputTags::ShouldReconcile(const FMoverDataStructBase& AuthorityState) const
{
	const FRPG_MoverInputTags& TypedAuthority = static_cast<const FRPG_MoverInputTags&>(AuthorityState);
	return *this != TypedAuthority;
}

void FRPG_RawInput::Interpolate(const FMoverDataStructBase& From, const FMoverDataStructBase& To, float Pct)
{
	const FRPG_RawInput& TypedFrom = static_cast<const FRPG_RawInput&>(From);
	const FRPG_RawInput& TypedTo = static_cast<const FRPG_RawInput&>(To);
	
	MoveInput = FMath::Lerp(TypedFrom.MoveInput, TypedTo.MoveInput, Pct);
}

void FRPG_MoverInputTags::Interpolate(const FMoverDataStructBase& From, const FMoverDataStructBase& To, float Pct)
{
	
}

void FRPG_MoverInputTags::Merge(const FMoverDataStructBase& From)
{
	
}

bool FRPG_RawInput::ShouldReconcile(const FMoverDataStructBase& AuthorityState) const
{
	const FRPG_RawInput& TypedAuthority = static_cast<const FRPG_RawInput&>(AuthorityState);
	return *this != TypedAuthority;
}

void FRPG_RawInput::Merge(const FMoverDataStructBase& From)
{
	
}

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

bool UMoverFunctionLibrary::IsMovementMode(UMoverComponent* Target, FName CompareName)
{
	if (!Target)return false;
	return Target->GetMovementModeName() == CompareName;
}

void UMoverFunctionLibrary::AddInputTags(FMoverDataCollection& Collection, FGameplayTagContainer InputTags)
{
	if (InputTags.Num() <= 0)return;
	FRPG_MoverInputTags& inputTagsData = Collection.FindOrAddMutableDataByType<FRPG_MoverInputTags>();
	inputTagsData.Tags = InputTags;
	//UBetterUtilities::DebugLog("Add Tag",EEasylog::Log);
}

bool UMoverFunctionLibrary::HasInputTag(FMoverDataCollection& Collection, FGameplayTag InputTag)
{
	if (InputTag.IsValid())
	{
		const FRPG_MoverInputTags* InputTags = Collection.FindDataByType<FRPG_MoverInputTags>();
		if (!InputTags)return false;
		//UBetterUtilities::DebugLog("Has Tag",EEasylog::Log);

		return InputTags->Tags.HasTag(InputTag);
		
	}
	return false;
}

void UMoverFunctionLibrary::AddRawInput(FMoverDataCollection& Collection, FRPG_RawInput RawInput)
{
	FRPG_RawInput& input= Collection.FindOrAddMutableDataByType<FRPG_RawInput>();
	input = RawInput;
	
}

FRPG_RawInput UMoverFunctionLibrary::GetRawInput(FMoverDataCollection& Collection)
{
	const FRPG_RawInput* Input = Collection.FindDataByType<FRPG_RawInput>();
	if (Input)
	{
		return *Input;
	}
	return FRPG_RawInput();
}
