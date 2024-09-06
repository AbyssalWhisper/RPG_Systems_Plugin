// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/MovementComponent/RPG_MovementComponent.h"
#include "RPG_Systems/MovementComponent/RPG_BaseMovementMode.h"

// Sets default values for this component's properties
URPG_MovementComponent::URPG_MovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URPG_MovementComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentMovementMode = *MovementModes.Find(StartMovementMode);
	// ...
	
}


// Called every frame
void URPG_MovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CurrentMovementMode)
	{
		CurrentMovementMode->PerformMovement(DirectionInput, DeltaTime,this);
	}
	// ...
	LastDirectionInput = DirectionInput;
	DirectionInput = FVector();
}

void URPG_MovementComponent::AddInput(FVector Input)
{
	DirectionInput += Input;
}

