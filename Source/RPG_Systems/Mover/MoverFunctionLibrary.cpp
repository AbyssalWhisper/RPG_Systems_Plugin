// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverFunctionLibrary.h"
#include "MoveLibrary/MovementUtils.h"
#include <BetterUtilitiesBPLibrary.h>

void UMoverFunctionLibrary::SetTransforms_WorldSpace(FMoverTickEndData& OutputState, FVector WorldLocation,
	FRotator WorldOrient, FVector WorldVelocity, UPrimitiveComponent* Base, FName BaseBone)
{
	FMoverDefaultSyncState& OutputSyncState = OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();
	OutputSyncState.SetTransforms_WorldSpace( WorldLocation,WorldOrient,WorldVelocity);
}

void UMoverFunctionLibrary::DrawDebugCapsule(UObject* Context,FVector Start, FVector End, FRotator Rotation, float HalfHeigth, float Radius, float DrawTime)
{
	FEventRef TaskCompletedEvent(EEventMode::ManualReset);
	
	AsyncTask(ENamedThreads::GameThread, [ &TaskCompletedEvent, Context,Start, End, Rotation, HalfHeigth, Radius, DrawTime]()
		{
			UWorld* world = Context->GetWorld();
			if (Context&&world)
			{
				
				UKismetSystemLibrary::DrawDebugCapsule(world,Start, HalfHeigth, Radius, Rotation, FLinearColor::Red, world->DeltaTimeSeconds, DrawTime);
				UKismetSystemLibrary::DrawDebugCapsule(world, End, HalfHeigth, Radius, Rotation, FLinearColor::Red, world->DeltaTimeSeconds, DrawTime);

			}

			// Coloque aqui o código que precisa rodar no Game Thread
			TaskCompletedEvent->Trigger();
		});
	TaskCompletedEvent->Wait();
}

void UMoverFunctionLibrary::DrawDebugLine(UObject* Context,FVector Start, FVector End, float DrawTime)
{
	FEventRef TaskCompletedEvent(EEventMode::ManualReset);
	AsyncTask(ENamedThreads::GameThread, [&TaskCompletedEvent, Context,Start, End, DrawTime]()
		{
			UWorld* world = Context->GetWorld();
			if (Context&&world)
			{
				UKismetSystemLibrary::DrawDebugLine(world, Start, End, FLinearColor::Red, world->DeltaTimeSeconds, DrawTime);
			}

			// Coloque aqui o código que precisa rodar no Game Thread
			TaskCompletedEvent->Trigger();
		});
	TaskCompletedEvent->Wait();
}