// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/BaseMovementMode/PhysicsDrivenClimbMode.h"

#include "MoverComponent.h"
#include "DefaultMovementSet/Settings/CommonLegacyMovementSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MoveLibrary/AirMovementUtils.h"

void UPhysicsDrivenClimbMode::OnGenerateMove(const FMoverTickStartData& StartState, const FMoverTimeStep& TimeStep,
                                             FProposedMove& OutProposedMove) const
{
	
	const FCharacterDefaultInputs* CharacterInputs = StartState.InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();
	const FMoverDefaultSyncState* StartingSyncState = StartState.SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();
	check(StartingSyncState);
	auto OwnerActor = GetMoverComponent()->GetOwner();
	const float DeltaSeconds = GetDeltaSecondsFromTimeStep(TimeStep);

	FFreeMoveParams Params;
	if (CharacterInputs)
	{
		FVector MoveInput;
		MoveInput = CharacterInputs->GetMoveInput();
		Params.MoveInputType = CharacterInputs->GetMoveInputType();
		//Params.MoveInput = CharacterInputs->GetMoveInput();
		// Configurar aqui a direção da movimentação do personagem 
		Params.MoveInput = (OwnerActor->GetActorRightVector()*MoveInput.Y)
		+ (OwnerActor->GetActorUpVector()*MoveInput.X)
		+ (OwnerActor->GetActorForwardVector()*DeltaSeconds*50);
	}
	else
	{
		Params.MoveInputType = EMoveInputType::Invalid;
		Params.MoveInput = FVector::ZeroVector;
	}

	
	FRotator IntendedOrientation_WorldSpace;
	
	if (OwnerActor)
	{
		FHitResult Hit;  
		// Define the start and end locations for the line trace
		FVector StartLocation = OwnerActor->GetActorLocation();
		FVector EndLocation = StartLocation + OwnerActor->GetActorForwardVector() * 100.0f;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(OwnerActor); 
		GEngine->AddOnScreenDebugMessage(-1,0,FColor::Red,"Test");
		
		bool IsHiting = GetWorld()->LineTraceSingleByChannel(Hit,StartLocation,EndLocation,TraceChannel,CollisionParams,FCollisionResponseParams(ECollisionResponse::ECR_Block));
		// Draw the debug line
		//FColor LineColor = IsHiting ? FColor::Green : FColor::Red;
		//DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, DeltaSeconds, 0, 1.0f);

		if (IsHiting)
		{
			GEngine->AddOnScreenDebugMessage(-1,0,FColor::Red,OwnerActor->GetName());
		
			IntendedOrientation_WorldSpace = (Hit.ImpactNormal * -1).ToOrientationRotator();
		}
		else
		{
			IntendedOrientation_WorldSpace = StartingSyncState->GetOrientation_WorldSpace();
		}
		
		// Cria um evento para sincronização
		FEventRef TaskCompletedEvent(EEventMode::ManualReset);
		AsyncTask(ENamedThreads::GameThread, [this,&TaskCompletedEvent, StartLocation,EndLocation]()
			{
				if (!GetWorld())
				{
					UKismetSystemLibrary::DrawDebugLine(this, StartLocation, EndLocation, FLinearColor::Red, GetWorld()->DeltaTimeSeconds, 0.5f);

				}
				
				// Coloque aqui o código que precisa rodar no Game Thread
				TaskCompletedEvent->Trigger();
			});
		TaskCompletedEvent->Wait();
	}
	
	/*
	// If there's no intent from input to change orientation, use the current orientation
	if (!CharacterInputs || CharacterInputs->OrientationIntent.IsNearlyZero())
	{
		IntendedOrientation_WorldSpace = StartingSyncState->GetOrientation_WorldSpace();
	}
	else
	{
		IntendedOrientation_WorldSpace = CharacterInputs->GetOrientationIntentDir_WorldSpace().ToOrientationRotator();
	}
	*/

	
	
	Params.OrientationIntent = IntendedOrientation_WorldSpace;
	Params.PriorVelocity = StartingSyncState->GetVelocity_WorldSpace();
	
	//Params.PriorVelocity = FVector(1000*DeltaSeconds,0,0)+InputDirection;
	Params.PriorOrientation = StartingSyncState->GetOrientation_WorldSpace();
	Params.TurningRate = CommonLegacySettings->TurningRate;
	Params.TurningBoost = CommonLegacySettings->TurningBoost;
	Params.MaxSpeed = CommonLegacySettings->MaxSpeed;
	Params.Acceleration = CommonLegacySettings->Acceleration;
	Params.Deceleration = CommonLegacySettings->Deceleration;
	Params.DeltaSeconds = DeltaSeconds;
	
	OutProposedMove = UAirMovementUtils::ComputeControlledFreeMove(Params);

	
}

void UPhysicsDrivenClimbMode::DrawDebug()
{
	auto OwnerActor = GetMoverComponent()->GetOwner();
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	if (OwnerActor)
	{
		FHitResult Hit;  
		// Define the start and end locations for the line trace
		FVector StartLocation = OwnerActor->GetActorLocation();
		FVector EndLocation = StartLocation + OwnerActor->GetActorForwardVector() * 100.0f;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(OwnerActor);
		TArray<AActor*> aa;
		GEngine->AddOnScreenDebugMessage(-1,DeltaSeconds,FColor::Blue,"Test");
		UKismetSystemLibrary::LineTraceSingle(OwnerActor,StartLocation,EndLocation,TraceTypeQuery1,false,aa,EDrawDebugTrace::None,Hit,true);
		//bool IsHiting = GetWorld()->LineTraceSingleByChannel(Hit,StartLocation,EndLocation,TraceChannel,CollisionParams,FCollisionResponseParams(ECollisionResponse::ECR_Block));
		// Draw the debug line
		//FColor LineColor = IsHiting ? FColor::Green : FColor::Red;
		//DrawDebugLine(GetWorld(), StartLocation, EndLocation, LineColor, false, DeltaSeconds, 0, 1.0f);
 
		
	}
}
