// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverFunctionLibrary.h"
#include "MoveLibrary/MovementUtils.h"

void UMoverFunctionLibrary::SetTransforms_WorldSpace(FMoverTickEndData& OutputState, FVector WorldLocation,
	FRotator WorldOrient, FVector WorldVelocity, UPrimitiveComponent* Base, FName BaseBone)
{
	FMoverDefaultSyncState& OutputSyncState = OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();
	OutputSyncState.SetTransforms_WorldSpace( WorldLocation,WorldOrient,WorldVelocity);
}
