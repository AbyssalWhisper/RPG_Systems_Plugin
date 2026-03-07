#include "RPG_Systems/Mover/ChaosCharacterMovementModeBP_Library.h"

#include "ChaosMover/Character/Modes/ChaosCharacterMovementMode.h"

bool UChaosCharacterMovementModeBP_Library::ShouldEnableConstraint(const UChaosCharacterMovementMode* MovementMode)
{
	return MovementMode ? MovementMode->ShouldEnableConstraint() : false;
}

float UChaosCharacterMovementModeBP_Library::GetTargetHeight(const UChaosCharacterMovementMode* MovementMode)
{
	return MovementMode ? MovementMode->GetTargetHeight() : 0.0f;
}

float UChaosCharacterMovementModeBP_Library::GetGroundQueryRadius(const UChaosCharacterMovementMode* MovementMode)
{
	return MovementMode ? MovementMode->GetGroundQueryRadius() : 0.0f;
}

float UChaosCharacterMovementModeBP_Library::GetMaxWalkSlopeCosine(const UChaosCharacterMovementMode* MovementMode)
{
	return MovementMode ? MovementMode->GetMaxWalkSlopeCosine() : 0.0f;
}

bool UChaosCharacterMovementModeBP_Library::ShouldCharacterRemainUpright(const UChaosCharacterMovementMode* MovementMode)
{
	return MovementMode ? MovementMode->ShouldCharacterRemainUpright() : false;
}

float UChaosCharacterMovementModeBP_Library::GetMaxSpeed(const UChaosCharacterMovementMode* MovementMode)
{
	return MovementMode ? MovementMode->GetMaxSpeed() : 0.0f;
}

void UChaosCharacterMovementModeBP_Library::OverrideMaxSpeed(UChaosCharacterMovementMode* MovementMode, float Value)
{
	if (MovementMode)
	{
		MovementMode->OverrideMaxSpeed(Value);
	}
}

void UChaosCharacterMovementModeBP_Library::ClearMaxSpeedOverride(UChaosCharacterMovementMode* MovementMode)
{
	if (MovementMode)
	{
		MovementMode->ClearMaxSpeedOverride();
	}
}

float UChaosCharacterMovementModeBP_Library::GetAcceleration(const UChaosCharacterMovementMode* MovementMode)
{
	return MovementMode ? MovementMode->GetAcceleration() : 0.0f;
}

void UChaosCharacterMovementModeBP_Library::OverrideAcceleration(UChaosCharacterMovementMode* MovementMode, float Value)
{
	if (MovementMode)
	{
		MovementMode->OverrideAcceleration(Value);
	}
}

void UChaosCharacterMovementModeBP_Library::ClearAccelerationOverride(UChaosCharacterMovementMode* MovementMode)
{
	if (MovementMode)
	{
		MovementMode->ClearAccelerationOverride();
	}
}
