// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/BaseMovementMode/PhysicsBaseMovementMode.h"

#include "MoverComponent.h"
#include "Math/UnitConversion.h"


#if WITH_EDITOR
#include "Backends/MoverNetworkPhysicsLiaison.h"
#include "Internationalization/Text.h"
#include "Misc/DataValidation.h"
#endif // WITH_EDITOR

#define LOCTEXT_NAMESPACE "BasePhysicsDrivenMode"


void UPhysicsBaseMovementMode::UpdateConstraintSettings(Chaos::FCharacterGroundConstraint& Constraint) const
{
	Constraint.SetRadialForceLimit(300000.0); // TEMP - Move radial force limit to shared mode data
	Constraint.SetTwistTorqueLimit(FUnitConversion::Convert(TwistTorqueLimit, EUnit::NewtonMeters, EUnit::KilogramCentimetersSquaredPerSecondSquared));
	Constraint.SetSwingTorqueLimit(FUnitConversion::Convert(SwingTorqueLimit, EUnit::NewtonMeters, EUnit::KilogramCentimetersSquaredPerSecondSquared));
	Constraint.SetTargetHeight(0.0f);
}
#if WITH_EDITOR
EDataValidationResult UPhysicsBaseMovementMode::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);
	if (!GetOuter())return Result;
	auto Mover = Cast<UMoverComponent>(GetOuter());
	if (!Mover)return Result;
	const UClass* BackendClass = GetMoverComponent()->BackendClass;
	if (BackendClass && !BackendClass->IsChildOf<UMoverNetworkPhysicsLiaisonComponent>())
	{
		Context.AddError(LOCTEXT("PhysicsMovementModeHasValidPhysicsLiaison", "Physics movement modes need to have a backend class that supports physics (UMoverNetworkPhysicsLiaisonComponent)."));
		Result = EDataValidationResult::Invalid;
	}
		
	return Result;
}

#endif // WITH_EDITOR
float UPhysicsBaseMovementMode::GetDeltaSecondsFromParams(const FSimulationTickParams& Params)
{
	return Params.TimeStep.StepMs * 0.001f;
}