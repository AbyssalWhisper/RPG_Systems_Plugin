// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Mover/RPG_BaseMoverCharacter.h"

#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "MoveLibrary/BasedMovementUtils.h"

#include "Components/InputComponent.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Backends/MoverNetworkPhysicsLiaison.h"

#include "Net/UnrealNetwork.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "PhysicsMover/Modes/PhysicsDrivenWalkingMode.h"

#include "Components/CapsuleComponent.h"

#include "RPG_Systems/GameplayAbility/RPG_BaseAttributeSet.h"

#include "EnhancedInputSubsystems.h"
#include "MovementComponents/RPG_CharacterMoverComponent.h"
/*
void ARPG_MoverBaseCharacter::OnProduceInput(float DeltaMs, FMoverInputCmdContext& InputCmdResult)
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
		//Verifica se deve rotacionar para a dire��o do movimento 
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
void ARPG_BaseMoverCharacter::TryCrouch()
{
	bWantsCrouch = true;
}

void ARPG_BaseMoverCharacter::TryUnCrouch()
{
	bWantsCrouch = false;
}

void ARPG_BaseMoverCharacter::TryJump()
{
	bIsJumpJustPressed = !bIsJumpPressed;
	bIsJumpPressed = true;
}
 


ARPG_BaseMoverCharacter::ARPG_BaseMoverCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//Gas
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);
	AttributesSet = CreateDefaultSubobject<URPG_BaseAttributeSet>("PlayerAttributes");


	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
	RootComponent = CapsuleComp;
	
	CharacterMotionComponent = CreateDefaultSubobject<URPG_CharacterMoverComponent>(CharacterMovementComponentName);

	//ensure(CharacterMotionComponent);

	PrimaryActorTick.bCanEverTick = true;

	SetReplicatingMovement(false);	// disable Actor-level movement replication, since our Mover component will handle it

	auto IsImplementedInBlueprint = [](const UFunction* Func) -> bool
		{
			return Func && ensure(Func->GetOuter())
				&& Func->GetOuter()->IsA(UBlueprintGeneratedClass::StaticClass());
		};


	static FName ProduceInputBPFuncName = FName(TEXT("OnProduceInputInBlueprint"));
	UFunction* ProduceInputFunction = GetClass()->FindFunctionByName(ProduceInputBPFuncName);
	bHasProduceInputinBpFunc = IsImplementedInBlueprint(ProduceInputFunction);
}

UAbilitySystemComponent* ARPG_BaseMoverCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

#pragma region GameplayTags
void ARPG_BaseMoverCharacter::AddGameplayTag(FGameplayTag TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
}

void ARPG_BaseMoverCharacter::RemoveGameplayTag(FGameplayTag TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);

}
#pragma endregion

bool ARPG_BaseMoverCharacter::IsCrouching() const
{
	return bIsCrouching;
}

void ARPG_BaseMoverCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(ARPG_BaseCharacter, MovementStates);
	DOREPLIFETIME(ARPG_BaseMoverCharacter, bIsDead);
	DOREPLIFETIME(ARPG_BaseMoverCharacter, bIsEating);
	DOREPLIFETIME(ARPG_BaseMoverCharacter, bIsDrinking);
}

void ARPG_BaseMoverCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (Controller)
	{
		if (auto PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}



// Called every frame
void ARPG_BaseMoverCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Do whatever you want here. By now we have the latest movement state and latest input processed.


	// Spin camera based on input
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		// Simple input scaling. A real game will probably map this to an acceleration curve
		static float LookRateYaw = 100.f;	// degs/sec
		static float LookRatePitch = 100.f;	// degs/sec

		PC->AddYawInput(CachedLookInput.Yaw * LookRateYaw * DeltaTime);
		PC->AddPitchInput(-CachedLookInput.Pitch * LookRatePitch * DeltaTime);
	}

	// Clear all camera-related cached input
	CachedLookInput = FRotator::ZeroRotator;
	{
		/*
		if (CharacterMotionComponent->GetMovementModeName() == DefaultModeNames::Walking) {
		if (bWantsCrouch)
		{
			CapsuleComp->SetCapsuleSize(CrouchTargetCapsuleRadius, CrouchTargetCapsuleHalfHeight);
		}
		else
		{
			CapsuleComp->SetCapsuleSize(TargetCapsuleRadius, TargetCapsuleHalfHeight);

		}*/
	}
	

}

void ARPG_BaseMoverCharacter::BeginPlay()
{
	Super::BeginPlay(); 


	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		PC->PlayerCameraManager->ViewPitchMax = 89.0f;
		PC->PlayerCameraManager->ViewPitchMin = -89.0f;
	}
}





#pragma region Input

// Called to bind functionality to input
void ARPG_BaseMoverCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup some bindings - we are currently using Enhanced Input and just using some input actions assigned in editor for simplicity
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ARPG_BaseMoverCharacter::OnMoveTriggered);
		Input->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &ARPG_BaseMoverCharacter::OnMoveCompleted);
		Input->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ARPG_BaseMoverCharacter::OnLookTriggered);
		Input->BindAction(LookInputAction, ETriggerEvent::Completed, this, &ARPG_BaseMoverCharacter::OnLookCompleted);
		Input->BindAction(FlyInputAction, ETriggerEvent::Triggered, this, &ARPG_BaseMoverCharacter::OnFlyTriggered);
	}
}


void ARPG_BaseMoverCharacter::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult)
{
	OnProduceInput((float)SimTimeMs, InputCmdResult);

	if (bHasProduceInputinBpFunc)
	{
		InputCmdResult = OnProduceInputInBlueprint((float)SimTimeMs, InputCmdResult);
	}
}


void ARPG_BaseMoverCharacter::OnProduceInput(float DeltaMs, FMoverInputCmdContext& OutInputCmd)
{

	// Generate user commands. Called right before the Character movement simulation will tick (for a locally controlled pawn)
	// This isn't meant to be the best way of doing a camera system. It is just meant to show a couple of ways it may be done
	// and to make sure we can keep distinct the movement, rotation, and view angles.
	// Styles 1-3 are really meant to be used with a gamepad.
	//
	// Its worth calling out: the code that happens here is happening *outside* of the Character movement simulation. All we are doing
	// is generating the input being fed into that simulation. That said, this means that A) the code below does not run on the server
	// (and non controlling clients) and B) the code is not rerun during reconcile/resimulates. Use this information guide any
	// decisions about where something should go (such as aim assist, lock on targeting systems, etc): it is hard to give absolute
	// answers and will depend on the game and its specific needs. In general, at this time, I'd recommend aim assist and lock on 
	// targeting systems to happen /outside/ of the system, i.e, here. But I can think of scenarios where that may not be ideal too.

	FCharacterDefaultInputs& CharacterInputs = OutInputCmd.InputCollection.FindOrAddMutableDataByType<FCharacterDefaultInputs>();

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


	CharacterInputs.ControlRotation = FRotator::ZeroRotator;

	// Obter rota��o do controle 
	{
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC)
		{
			CharacterInputs.ControlRotation = PC->GetControlRotation();
		}

	}

	// Favor velocity input 
	bool bUsingInputIntentForMove = CachedMoveInputVelocity.IsZero();

	if (bUsingInputIntentForMove)
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
		if (bOrientRotationToMovement)
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

	// Clear/consume temporal movement inputs. We are not consuming others in the event that the game world is ticking at a lower rate than the Mover simulation. 
	// In that case, we want most input to carry over between simulation frames.
	{

		bIsJumpJustPressed = false;
		bIsJumpPressed = false;
		bShouldToggleFlying = false;
	}
}




void ARPG_BaseMoverCharacter::OnMoveTriggered(const FInputActionValue& Value)
{
	const FVector MovementVector = Value.Get<FVector>();
	CachedMoveInputIntent.X = FMath::Clamp(MovementVector.X, -1.0f, 1.0f);
	CachedMoveInputIntent.Y = FMath::Clamp(MovementVector.Y, -1.0f, 1.0f);
	CachedMoveInputIntent.Z = FMath::Clamp(MovementVector.Z, -1.0f, 1.0f);
}
void ARPG_BaseMoverCharacter::OnMoveCompleted(const FInputActionValue& Value)
{
	CachedMoveInputIntent = FVector::ZeroVector;
}

void ARPG_BaseMoverCharacter::OnLookTriggered(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	CachedLookInput.Yaw = CachedTurnInput.Yaw = FMath::Clamp(LookVector.X, -1.0f, 1.0f);
	CachedLookInput.Pitch = CachedTurnInput.Pitch = FMath::Clamp(LookVector.Y, -1.0f, 1.0f);
}

void ARPG_BaseMoverCharacter::OnLookCompleted(const FInputActionValue& Value)
{
	CachedLookInput = FRotator::ZeroRotator;
	CachedTurnInput = FRotator::ZeroRotator;
}



void ARPG_BaseMoverCharacter::OnFlyTriggered(const FInputActionValue& Value)
{
	bShouldToggleFlying = true;
}
#pragma endregion

#pragma region Posses
void ARPG_BaseMoverCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (AbilitySystemComp)
	{
		AbilitySystemComp->RefreshAbilityActorInfo();
	}
	if (auto a = Cast<APlayerController>(NewController))
	{
		Client_PossessedByPlayer(a);
	}
}

void ARPG_BaseMoverCharacter::UnPossessed()
{
	Super::UnPossessed();
	if (auto a = Cast<APlayerController>(GetController()))
	{
		Client_UnPossessedByPlayer(a);
	}
}

void ARPG_BaseMoverCharacter::Client_PossessedByPlayer_Implementation(APlayerController* PlayerController)
{
	PossessedByPlayerOnClient(PlayerController);
}

void ARPG_BaseMoverCharacter::Client_UnPossessedByPlayer_Implementation(APlayerController* OldPlayerController)
{
	UnPossessedByPlayerOnClient(OldPlayerController);
}

void ARPG_BaseMoverCharacter::PossessedByPlayerOnClient_Implementation(APlayerController* PlayerController)
{
}

void ARPG_BaseMoverCharacter::UnPossessedByPlayerOnClient_Implementation(APlayerController* OldPlayerController)
{
}

#pragma endregion
