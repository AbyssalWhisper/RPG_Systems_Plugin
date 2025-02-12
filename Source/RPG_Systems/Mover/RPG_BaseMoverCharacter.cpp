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
#include "BetterUtilitiesBPLibrary.h"

#include "PhysicsMover/Modes/PhysicsDrivenWalkingMode.h"

#include "Components/CapsuleComponent.h"

#include "RPG_Systems/GameplayAbility/RPG_BaseAttributeSet.h"

#include "EnhancedInputSubsystems.h"
#include "MovementComponents/RPG_CharacterMoverComponent.h"
#include "RPG_Systems/GameplayAbility/RPG_GameplayAbilitySet.h"

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


void ARPG_BaseMoverCharacter::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult)
{
	InputCmdResult = OnProduceInputInBlueprint(SimTimeMs,InputCmdResult);
}

ARPG_BaseMoverCharacter::ARPG_BaseMoverCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass("MoverComponent", URPG_CharacterMoverComponent::StaticClass()))
	 
{
	//Gas
	AbilitySystemComp = CreateDefaultSubobject<URPG_AbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);
	AttributesSet = CreateDefaultSubobject<URPG_BaseAttributeSet>("PlayerAttributes");

	CharacterMoverComponent = CreateDefaultSubobject<URPG_CharacterMoverComponent>("CharacterMoverComponent");
	CharacterMoverComponent->SetIsReplicated(true);
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
	CapsuleComp->SetCollisionProfileName("BlockAll");
	RootComponent = CapsuleComp;
	 
 
 
}

void ARPG_BaseMoverCharacter::OnAbilityInputPressed(int32 InputID)
{
	if (AbilitySystemComp)
	{
		AbilitySystemComp->AbilityLocalInputPressed(InputID);
	}
}

void ARPG_BaseMoverCharacter::OnAbilityInputReleased(int32 InputID)
{
	if (AbilitySystemComp)
	{
		AbilitySystemComp->AbilityLocalInputReleased(InputID);
	}
}

UAbilitySystemComponent* ARPG_BaseMoverCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

#pragma region GameplayTags
void ARPG_BaseMoverCharacter::AddGameplayTag(FGameplayTag TagToAdd, int Count)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, Count);
}

void ARPG_BaseMoverCharacter::RemoveGameplayTag(FGameplayTag TagToRemove, int Count)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove,Count);

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
			CurrentPlayerController = PlayerController;
			CurrentPlayerController->PlayerCameraManager->ViewPitchMax = 89.0f;
			CurrentPlayerController->PlayerCameraManager->ViewPitchMin = -89.0f;
		}
		else
		{
			CurrentPlayerController = nullptr;
		}
	}
}



// Called every frame
void ARPG_BaseMoverCharacter::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

	UpdateCameraRotation(DeltaTime);
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


	

	if (PlayerAbilitiesDataAsset)
	{
		const TSet<FGameplayInputAbilityInfo>& InputAbilities = PlayerAbilitiesDataAsset->GetInputAbilities();
		for (auto Element : InputAbilities)
		{
			AbilitySystemComp->GiveAbilityWithInputAction(Element.GameplayAbilityClass,Element.InputAction);
		}
		
	}
}

FVector2f ARPG_BaseMoverCharacter::GetCameraInput_Implementation()
{
	return FVector2f(CachedLookInput.Yaw, -CachedLookInput.Pitch);
}



#pragma region Input
void ARPG_BaseMoverCharacter::UpdateCameraRotation(float DeltaTime)
{
	if (CurrentPlayerController)
	{
		FVector2f CameraInput = GetCameraInput();// Simple input scaling. A real game will probably map this to an acceleration curve

		static float LookRateYaw = 100.f;	// degs/sec
		static float LookRatePitch = 100.f;	// degs/sec

		CurrentPlayerController->AddYawInput(CameraInput.X * LookRateYaw * DeltaTime);
		CurrentPlayerController->AddPitchInput(CameraInput.Y * LookRatePitch * DeltaTime);
		
	}
}

// Called to bind functionality to input
void ARPG_BaseMoverCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup some bindings - we are currently using Enhanced Input and just using some input actions assigned in editor for simplicity
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		/*
		if (PlayerAbilitiesDataAsset)
		{
			const TSet<FGameplayInputAbilityInfo>& InputAbilities = PlayerAbilitiesDataAsset->GetInputAbilities();
			for (const auto& It : InputAbilities)
			{
				if (It.IsValid())
				{
					UInputAction* InputAction = It.InputAction;
					
					UBetterUtilities::DebugLog("Binding InputAction: " + InputAction->GetFName().ToString());
					AbilitySystemComp->GiveAbilityWithInputAction(It.GameplayAbilityClass, InputAction);
				}
			}
		}*/
	}
	
}

URPG_CharacterMoverComponent* ARPG_BaseMoverCharacter::GetMoverComponent() const
{
	return CharacterMoverComponent;
}


FVector ARPG_BaseMoverCharacter::ProcessMoveInput_Implementation(const FVector& MoveInput)
{
	return MoveInput;
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
