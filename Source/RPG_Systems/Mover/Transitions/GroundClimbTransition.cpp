// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/Transitions/GroundClimbTransition.h"
#include "MoveLibrary/MovementUtils.h"
#include "BetterUtilitiesBPLibrary.h"
#include "MoverComponent.h"
#include "Components/CapsuleComponent.h"
#include "BetterUtilities/Public/TraceUtils.h"

FTransitionEvalResult UGroundClimbTransition::OnEvaluate(const FSimulationTickParams& Params) const
{
	
	FTransitionEvalResult Transition;
	const FCharacterDefaultInputs* CharacterInputs = Params.StartState.InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, CharacterInputs->GetMoveInput().ToString());

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
				TraceChannel, false, a, EDrawDebugTrace::ForDuration, Hit, true);
			if (bHit)
			{
				return Transition.NextMode = "Climb";
			}
		}
	}
	return Transition.NoTransition;
	//return FTransitionEvalResult();
}
