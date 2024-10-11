// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_Systems/Mover/MovementComponents/RPG_CharacterMoverComponent.h"
#include "PhysicsMover/Modes/PhysicsDrivenWalkingMode.h"
#include "PhysicsMover/Modes/PhysicsDrivenSwimmingMode.h"
#include "Backends/MoverNetworkPhysicsLiaison.h"

URPG_CharacterMoverComponent::URPG_CharacterMoverComponent()
{
	BackendClass = UMoverNetworkPhysicsLiaisonComponent::StaticClass();

MovementModes.Add(DefaultModeNames::Swimming, CreateDefaultSubobject<USwimmingMode>(TEXT("DefaultSwimmingMode")));
	//MovementModes.Add(RPG_MovementModes::Climbing, CreateDefaultSubobject<UPhysicsDrivenClimbMode>(TEXT("PhysicsDrivenClimbingMode")));
	//MovementModes.Find(DefaultModeNames::Walking)->Get()->Transitions.Add(CreateDefaultSubobject<UPhysicsJumpCheck>(TEXT("PhysicsJumpCheck")));
	//MovementModes.Find(DefaultModeNames::Walking)->Get()->Transitions.Add(CreateDefaultSubobject<UGroundClimbTransition>(TEXT("GroundClimbTransition")));
	

}




void URPG_CharacterMoverComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ARPG_BaseMoverCharacter>(GetOwner());
	
}

void URPG_CharacterMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

TObjectPtr<UBaseMovementMode> URPG_CharacterMoverComponent::GetCurrentMovementMode()
{
	if (bHasValidCachedState)
	{
		
		return MovementModes.Find(CachedLastSyncState.MovementMode)->Get();
	}
	return nullptr;
}


bool URPG_CharacterMoverComponent::IsClimbing() const
{
	if (bHasValidCachedState)
	{
		return CachedLastSyncState.MovementMode == DefaultModeNames::Swimming;
	}

	return false;
}
