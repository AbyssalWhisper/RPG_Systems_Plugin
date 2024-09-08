// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/Transitions/GroundClimbTransition.h"
#include "MoveLibrary/MovementUtils.h"
#include "BetterUtilitiesBPLibrary.h"
#include "MoverComponent.h"
#include "Components/CapsuleComponent.h"
#include "BetterUtilities/Public/TraceUtils.h"
#include "RPG_Systems/Mover/MoverFunctionLibrary.h"

FTransitionEvalResult UGroundClimbTransition::OnEvaluate(const FSimulationTickParams& Params) const
{
	
	FTransitionEvalResult Transition;

	if (!CanClimb)return Transition.NoTransition;

	const FCharacterDefaultInputs* CharacterInputs = Params.StartState.InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();
	
	if (CharacterInputs->GetMoveInput().IsNearlyZero())return Transition.NoTransition;
	APawn* pawn = Cast<APawn>(Params.MoverComponent->GetOwner());
	UCapsuleComponent*Capsule = pawn->GetComponentByClass<UCapsuleComponent>();
	if (pawn && pawn->IsLocallyControlled())
	{
		if (Params.MoverComponent->GetVelocity().Length() <= 50)
		{
			FHitResult Hit;
			TArray<AActor*> a;
			float CapsuleRadius = Capsule->GetScaledCapsuleRadius();
			bool bHit = UTraceUtils::CapsuleTraceSingle(pawn, pawn->GetActorLocation(), pawn->GetActorLocation() + Capsule->GetForwardVector() * CapsuleRadius,
				CapsuleRadius, Capsule->GetScaledCapsuleHalfHeight(), Capsule->GetComponentRotation(),
				TraceChannel, false, a, EDrawDebugTrace::None, Hit, true);
			UMoverFunctionLibrary::ChaosDrawDebugCapsuleRay(pawn->GetActorLocation(), pawn->GetActorLocation() + Capsule->GetForwardVector(),Capsule->GetScaledCapsuleHalfHeight(), CapsuleRadius, Capsule->GetComponentRotation());
			
			if (bHit)
			{
				return Transition.NextMode = "Climb";
			}
		}
	}
	return Transition.NoTransition;
	//return FTransitionEvalResult();
}
