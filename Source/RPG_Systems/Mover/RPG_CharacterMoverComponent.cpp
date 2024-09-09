// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/RPG_CharacterMoverComponent.h"
#include "PhysicsMover/Modes/PhysicsDrivenWalkingMode.h"
#include "PhysicsMover/Modes/PhysicsDrivenFallingMode.h"
#include "PhysicsMover/Modes/PhysicsDrivenFlyingMode.h"
#include "RPG_Systems/Mover/BaseMovementMode/PhysicsDrivenClimbMode.h"
#include "PhysicsMover/Transitions/PhysicsJumpCheck.h"


#include "Backends/MoverNetworkPhysicsLiaison.h"
#include "Transitions/GroundClimbTransition.h"


URPG_CharacterMoverComponent::URPG_CharacterMoverComponent()
{
 


	//BackendClass = UMoverNetworkPhysicsLiaisonComponent::StaticClass();



	MovementModes.Add(DefaultModeNames::Walking, CreateDefaultSubobject<UPhysicsDrivenWalkingMode>(TEXT("DefaultDrivenWalkingMode")));
	MovementModes.Add(DefaultModeNames::Falling, CreateDefaultSubobject<UPhysicsDrivenFallingMode>(TEXT("DefaultDrivenFallingMode")));
	MovementModes.Add(DefaultModeNames::Flying, CreateDefaultSubobject<UPhysicsDrivenFlyingMode>(TEXT("DefaultDrivenFlyingMode")));
	MovementModes.Add("Climb", CreateDefaultSubobject<UPhysicsDrivenClimbMode>(TEXT("PhysicsDrivenClimbMode")));
	MovementModes.Find(DefaultModeNames::Walking)->Get()->Transitions.Add(CreateDefaultSubobject<UPhysicsJumpCheck>(TEXT("PhysicsJumpCheck")));
	MovementModes.Find(DefaultModeNames::Walking)->Get()->Transitions.Add(CreateDefaultSubobject<UGroundClimbTransition>(TEXT("GroundClimbTransition")));
 
}

bool URPG_CharacterMoverComponent::IsSwimming() const
{
	if (bHasValidCachedState)
	{
		return CachedLastSyncState.MovementMode == DefaultModeNames::Swimming;
	}

	return false;

}bool URPG_CharacterMoverComponent::IsFlying() const
{
	if (bHasValidCachedState)
	{
		return CachedLastSyncState.MovementMode == DefaultModeNames::Flying;
	}

	return false;
}
