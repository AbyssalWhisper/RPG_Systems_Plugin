// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/RPG_BasePhysicsCharacter.h"

#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MoveLibrary/BasedMovementUtils.h"
/*
void ARPG_BasePhysicsCharacter::OnProduceInput(float DeltaMs, FMoverInputCmdContext& InputCmdResult)
{
	FCharacterDefaultInputs& CharacterInputs = InputCmdResult.InputCollection.FindOrAddMutableDataByType<FCharacterDefaultInputs>();

	if (Controller == nullptr)
	{
		if (GetLocalRole() == ENetRole::ROLE_Authority && GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
		{
			static const FCharacterDefaultInputs DoNothingInput;
			// If we get here, that means this pawn is not currently possessed and we're choosing to provide default do-nothing input
			CharacterInputs = DoNothingInput;
		}

		// We don't have a local controller so we can't run the code below. This is ok. Simulated proxies will just use previous input when extrapolating
		return;
	}

	if (USpringArmComponent* SpringComp = FindComponentByClass<USpringArmComponent>())
	{
		// This is not best practice: do not search for component every frame
		SpringComp->bUsePawnControlRotation = true;
	}

	CharacterInputs.ControlRotation = FRotator::ZeroRotator;

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		CharacterInputs.ControlRotation = PC->GetControlRotation();
	}

	// Favor velocity input 
	bool bUsingInputIntentForMove = CachedMoveInputVelocity.IsZero();

	if (bUsingInputIntentForMove)
	{
		if (IsClimbing()) {
			CharacterInputs.SetMoveInput(EMoveInputType::DirectionalIntent, CachedMoveInputIntent);
		}
		else
		{
			FRotator Rotator = CharacterInputs.ControlRotation;
			FVector FinalDirectionalIntent;
			if (const UCharacterMoverComponent* MoverComp = GetMoverComponent())
			{
				if (MoverComp->IsOnGround() || MoverComp->IsFalling())
				{
					const FVector RotationProjectedOntoUpDirection = FVector::VectorPlaneProject(Rotator.Vector(), MoverComp->GetUpDirection()).GetSafeNormal();
					Rotator = RotationProjectedOntoUpDirection.Rotation();
				}

				FinalDirectionalIntent = Rotator.RotateVector(CachedMoveInputIntent);
			}

			CharacterInputs.SetMoveInput(EMoveInputType::DirectionalIntent, FinalDirectionalIntent);
		}
	}
	else
	{
		CharacterInputs.SetMoveInput(EMoveInputType::Velocity, CachedMoveInputVelocity);
	}

	static float RotationMagMin(1e-3);

	const bool bHasAffirmativeMoveInput = (CharacterInputs.GetMoveInput().Size() >= RotationMagMin);
	
	// Figure out intended orientation
	CharacterInputs.OrientationIntent = FVector::ZeroVector;


	if (bUsingInputIntentForMove && bHasAffirmativeMoveInput)
	{
		//Verifica se deve rotacionar para a direção do movimento 
		if (bOrientRotationToMovement && !IsClimbing())
		{
			// set the intent to the actors movement direction
			CharacterInputs.OrientationIntent = CharacterInputs.GetMoveInput();
		 
			
		}
		else
		{
			// set intent to the the control rotation - often a player's camera rotation
			CharacterInputs.OrientationIntent = CharacterInputs.ControlRotation.Vector();
		}

		LastAffirmativeMoveInput = CharacterInputs.GetMoveInput();

	}
	else if (bMaintainLastInputOrientation)
	{
		// There is no movement intent, so use the last-known affirmative move input
		CharacterInputs.OrientationIntent = LastAffirmativeMoveInput;
	}
	
	if (bShouldRemainVertical)
	{
		// canceling out any z intent if the actor is supposed to remain vertical
		CharacterInputs.OrientationIntent = CharacterInputs.OrientationIntent.GetSafeNormal2D();
	}

	CharacterInputs.bIsJumpPressed = bIsJumpPressed;
	CharacterInputs.bIsJumpJustPressed = bIsJumpJustPressed;

	if (bShouldToggleFlying)
	{
		if (!bIsFlyingActive)
		{
			CharacterInputs.SuggestedMovementMode = DefaultModeNames::Flying;
		}
		else
		{
			CharacterInputs.SuggestedMovementMode = DefaultModeNames::Falling;
		}

		bIsFlyingActive = !bIsFlyingActive;
	}
	else
	{
		CharacterInputs.SuggestedMovementMode = NAME_None;
	}

	// Convert inputs to be relative to the current movement base (depending on options and state)
	CharacterInputs.bUsingMovementBase = false;

	if (bUseBaseRelativeMovement)
	{
		if (const UCharacterMoverComponent* MoverComp = GetComponentByClass<UCharacterMoverComponent>())
		{
			if (UPrimitiveComponent* MovementBase = MoverComp->GetMovementBase())
			{
				FName MovementBaseBoneName = MoverComp->GetMovementBaseBoneName();

				FVector RelativeMoveInput, RelativeOrientDir;

				UBasedMovementUtils::TransformWorldDirectionToBased(MovementBase, MovementBaseBoneName, CharacterInputs.GetMoveInput(), RelativeMoveInput);
				UBasedMovementUtils::TransformWorldDirectionToBased(MovementBase, MovementBaseBoneName, CharacterInputs.OrientationIntent, RelativeOrientDir);

				CharacterInputs.SetMoveInput(CharacterInputs.GetMoveInputType(), RelativeMoveInput);
				CharacterInputs.OrientationIntent = RelativeOrientDir;

				CharacterInputs.bUsingMovementBase = true;
				CharacterInputs.MovementBase = MovementBase;
				CharacterInputs.MovementBaseBoneName = MovementBaseBoneName;
			}
		}
	}

	
	if (bWantsCrouch && !IsCrouching())
	{
		CharacterInputs.SuggestedMovementMode = RPG_MovementModes::Crouch; 
	}
	else
	{
		CharacterInputs.SuggestedMovementMode = DefaultModeNames::Walking;
	}



	// Clear/consume temporal movement inputs. We are not consuming others in the event that the game world is ticking at a lower rate than the Mover simulation. 
	// In that case, we want most input to carry over between simulation frames.
	{

		bIsJumpJustPressed = false;
		bShouldToggleFlying = false;
	}


}
*/
void ARPG_BasePhysicsCharacter::TryCrouch()
{
	bWantsCrouch = true;
}

void ARPG_BasePhysicsCharacter::TryUnCrouch()
{
	bWantsCrouch = false;
}
 

bool ARPG_BasePhysicsCharacter::IsClimbing() const
{
	//if (GetMoverComponent())
	//{
	//	return GetMoverComponent()->GetMovementModeName() == RPG_MovementModes::Climb;
	//}

	return false;
}

bool ARPG_BasePhysicsCharacter::IsCrouching() const
{
	//if (GetMoverComponent())
	//{
	//	return GetMoverComponent()->GetMovementModeName() == RPG_MovementModes::Crouch;
	//}
	//
	return false;
}

bool ARPG_BasePhysicsCharacter::IsWalking() const
{
	//if (CharacterMotionComponent)
	//{
	//	return CharacterMotionComponent->IsOnGround();
	//}
	return false;
}
