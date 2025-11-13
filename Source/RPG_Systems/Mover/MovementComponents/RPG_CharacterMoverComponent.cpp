// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_Systems/Mover/MovementComponents/RPG_CharacterMoverComponent.h"

#include "BetterUtilitiesBPLibrary.h"
#include "PhysicsMover/Modes/PhysicsDrivenWalkingMode.h"
#include "PhysicsMover/Modes/PhysicsDrivenSwimmingMode.h"
#include "Backends/MoverNetworkPhysicsLiaison.h"
#include "Components/CapsuleComponent.h"
#include "DefaultMovementSet/InstantMovementEffects/BasicInstantMovementEffects.h"
#include "MoveLibrary/MovementUtils.h"
#include "RPG_Systems/Mover/MoverFunctionLibrary.h"

void URPG_CharacterMoverComponent::OnMoverPreSimulationTick(const FMoverTimeStep& TimeStep,
	const FMoverInputCmdContext& InputCmd)
{
	Super::OnMoverPreSimulationTick(TimeStep, InputCmd);

	const FCharacterDefaultInputs* CharacterInputs = InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();
	if (CharacterInputs && CharacterInputs->bIsJumpPressed)
	{
		TSharedPtr<FCapsuleSizeModifier> NewStanceModifier = MakeShared<FCapsuleSizeModifier>();
		//StanceModifierHandle = QueueMovementModifier(NewStanceModifier);

		
	}
}

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

bool URPG_CharacterMoverComponent::TeleportImmediately(const FVector& Location, const FRotator& Orientation,
	const FVector& Velocity)
{
	bool bSuccessfullyWrote = false;
	FMoverSyncState PendingSyncState;

	if (BackendLiaisonComp->ReadPendingSyncState(OUT PendingSyncState))
	{
		if (FMoverDefaultSyncState* DefaultSyncState = PendingSyncState.SyncStateCollection.FindMutableDataByType<FMoverDefaultSyncState>())
		{
			// Move the character and reflect this in the official simulation state
			UpdatedComponent->SetWorldLocationAndRotation(Location, Orientation);
			UpdatedComponent->ComponentVelocity = Velocity;
			DefaultSyncState->SetTransforms_WorldSpace(Location, Orientation, FVector::ZeroVector, nullptr);
			bSuccessfullyWrote = (BackendLiaisonComp->WritePendingSyncState(PendingSyncState));
			if (bSuccessfullyWrote)
			{
				FinalizeFrame(&PendingSyncState, &CachedLastAuxState);
			}
		}
	}
	return bSuccessfullyWrote;
}


void URPG_CharacterMoverComponent::OnHandleImpact(const FMoverOnImpactParams& ImpactParams)
{
	Super::OnHandleImpact(ImpactParams);
	// get the hit component
	UPrimitiveComponent* HitComponent = ImpactParams.HitResult.GetComponent();

	// is it valid and simulating physics?
	if (HitComponent && HitComponent->IsSimulatingPhysics())
	{
		FVector ImpactForce = ImpactParams.AttemptedMoveDelta * HitComponent->GetMass() * ImpactPhysicsForceMultiplier;
		HitComponent->AddImpulseAtLocation(ImpactForce, ImpactParams.HitResult.ImpactPoint);
		UBetterUtilities::DebugLog("Applied impact force on dynamic physics object",EEasylog::Log);
	}
}


bool URPG_CharacterMoverComponent::IsClimbing() const
{
	return HasGameplayTag(Mover_IsClimbing, true); 
}



FCapsuleSizeModifier::FCapsuleSizeModifier()
	: HalfHeight(0.0f)
{
	DurationMs = -1.0f; // Infinite duration by default
}

bool FCapsuleSizeModifier::HasGameplayTag(FGameplayTag TagToFind, bool bExactMatch) const
{
	return Super::HasGameplayTag(TagToFind, bExactMatch);
}

void FCapsuleSizeModifier::OnStart(UMoverComponent* MoverComp, const FMoverTimeStep& TimeStep,
	const FMoverSyncState& SyncState, const FMoverAuxStateContext& AuxState)
{
	if (const UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(MoverComp->GetUpdatedComponent()))
	{
		
		float OldHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
		
			
		AdjustCapsule(MoverComp, OldHalfHeight, HalfHeight, HalfHeight * 0.9f); // Assuming eye height is 90% of half height
		ApplyMovementSettings(MoverComp);
	}
}

void FCapsuleSizeModifier::OnEnd(UMoverComponent* MoverComp, const FMoverTimeStep& TimeStep,
	const FMoverSyncState& SyncState, const FMoverAuxStateContext& AuxState)
{
	const AActor* OwnerCDO = Cast<AActor>(MoverComp->GetOwner()->GetClass()->GetDefaultObject());

	if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(MoverComp->GetUpdatedComponent()))
	{
		if (const UCapsuleComponent* OriginalCapsule = UMovementUtils::GetOriginalComponentType<UCapsuleComponent>(MoverComp->GetOwner()))
		{
			if (const APawn* OwnerCDOAsPawn = Cast<APawn>(OwnerCDO))
			{
				AdjustCapsule(MoverComp, CapsuleComponent->GetScaledCapsuleHalfHeight(), OriginalCapsule->GetScaledCapsuleHalfHeight(), OwnerCDOAsPawn->BaseEyeHeight);
				RevertMovementSettings(MoverComp);
			}
		}
	}
}

void FCapsuleSizeModifier::OnPostSimulationTick(UMoverComponent* MoverComp,
                                                UPrimitiveComponent* UpdatedPrimitive)
{
}

FMovementModifierBase* FCapsuleSizeModifier::Clone() const
{
	FCapsuleSizeModifier* CopyPtr = new FCapsuleSizeModifier(*this);
	return CopyPtr;
}

void FCapsuleSizeModifier::NetSerialize(FArchive& Ar)
{
	Super::NetSerialize(Ar);
}

UScriptStruct* FCapsuleSizeModifier::GetScriptStruct() const
{
	return FCapsuleSizeModifier::StaticStruct();
}

FString FCapsuleSizeModifier::ToSimpleString() const
{
	return FString::Printf(TEXT("Capsule Size Modifier"));
}

void FCapsuleSizeModifier::AddReferencedObjects(class FReferenceCollector& Collector)
{
	Super::AddReferencedObjects(Collector);
}

bool FCapsuleSizeModifier::CanExpand_Internal(UMoverComponent* MoverComponent, USceneComponent* UpdatedComp, const FMoverSyncState& SyncState) const
{
	float StandingHalfHeight = 90;
	float CurrentHalfHeight = 55;

	bool bEncroached = true;

	if (const UCharacterMoverComponent* CharMoverComp = Cast<UCharacterMoverComponent>(MoverComponent))
	{
		const USceneComponent* UpdatedComponent = UpdatedComp;
		const UPrimitiveComponent* UpdatedCompAsPrimitive = Cast<UPrimitiveComponent>(UpdatedComponent);

		if (const UCapsuleComponent* OriginalCapsule = UMovementUtils::GetOriginalComponentType<UCapsuleComponent>(CharMoverComp->GetOwner()))
		{
			StandingHalfHeight = OriginalCapsule->GetScaledCapsuleHalfHeight();
		}

		if (const UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(UpdatedCompAsPrimitive))
		{
			CurrentHalfHeight = CapsuleComponent->GetScaledCapsuleHalfHeight();
		}

		const float HalfHeightDifference = StandingHalfHeight - CurrentHalfHeight;

		const FMoverDefaultSyncState* DefaultSyncState = SyncState.SyncStateCollection.FindDataByType<FMoverDefaultSyncState>();

		const FVector PawnLocation = DefaultSyncState->GetLocation_WorldSpace();
		const FQuat PawnRot = DefaultSyncState->GetOrientation_WorldSpace().Quaternion();
		float PawnRadius = 0.0f;
		float PawnHalfHeight = 0.0f;
		UpdatedCompAsPrimitive->CalcBoundingCylinder(PawnRadius, PawnHalfHeight);

		// TODO: Compensate for the difference between current capsule size and standing size
		const FCollisionShape StandingCapsuleShape = FCollisionShape::MakeCapsule(PawnRadius, StandingHalfHeight);
		const ECollisionChannel CollisionChannel = UpdatedCompAsPrimitive->GetCollisionObjectType();

		// TODO: @Harsha Switch to physics thread safe IsOnGround_Internal() method when available.
		bool bShouldMaintainBase = false;
		if (const TObjectPtr<UBaseMovementMode>* CurrentMode = CharMoverComp->MovementModes.Find(SyncState.MovementMode))
		{
			const UBaseMovementMode* ActiveMode = CurrentMode->Get();
			if (ActiveMode && ActiveMode->HasGameplayTag(Mover_IsOnGround, true))
			{
				bShouldMaintainBase = true;
			}
		}

		if (!bShouldMaintainBase)
		{
			// Expand in place
			bEncroached = UMovementUtils::OverlapTest(UpdatedComponent, UpdatedCompAsPrimitive, PawnLocation, PawnRot, CollisionChannel, StandingCapsuleShape, CharMoverComp->GetOwner());
		}
		else
		{
			// Expand while keeping base location the same.
			const FVector StandingLocation = PawnLocation + (HalfHeightDifference + .01f) * CharMoverComp->GetUpDirection();
			bEncroached = UMovementUtils::OverlapTest(UpdatedComponent, UpdatedCompAsPrimitive, StandingLocation, PawnRot, CollisionChannel, StandingCapsuleShape, CharMoverComp->GetOwner());
		}
	}
	return !bEncroached;
}

bool FCapsuleSizeModifier::ShouldExpandingMaintainBase(const UCharacterMoverComponent* MoverComp) const
{
	if (MoverComp->IsOnGround())
	{
		return true;
	}

	return false;
}

void FCapsuleSizeModifier::AdjustCapsule(UMoverComponent* MoverComp, float OldHalfHeight, float NewHalfHeight,
	float NewEyeHeight)
{
	const float HalfHeightDifference = FMath::Abs(NewHalfHeight - OldHalfHeight);
	const bool bExpanding = OldHalfHeight < NewHalfHeight;
	
	// Set capsule size to crouching size
	if (UCapsuleComponent* CapsuleComponent = Cast<UCapsuleComponent>(MoverComp->GetOwner()->FindComponentByClass(UCapsuleComponent::StaticClass())))
	{
		if (CapsuleComponent->GetUnscaledCapsuleHalfHeight() == NewHalfHeight)
		{
			return;
		}
		
		CapsuleComponent->SetCapsuleSize(CapsuleComponent->GetUnscaledCapsuleRadius(), NewHalfHeight);
	}

	// update eye height on pawn
	if (APawn* MoverCompOwnerAsPawn = Cast<APawn>(MoverComp->GetOwner()))
	{
		MoverCompOwnerAsPawn->BaseEyeHeight = NewEyeHeight;
	}

	const FVector CapsuleOffset = MoverComp->GetUpDirection() * (bExpanding ? HalfHeightDifference : -HalfHeightDifference);
	// This is only getting used to add relative offset - so assuming z is up is fine here
	const FVector VisualOffset = FVector::UpVector * (bExpanding ? -HalfHeightDifference : HalfHeightDifference);
	
	// Adjust location of capsule as setting it's size left it floating
	if (!bExpanding || MoverComp->GetVelocity().Length() <= 0)
	{
		TSharedPtr<FTeleportEffect> TeleportEffect = MakeShared<FTeleportEffect>();
		TeleportEffect->TargetLocation = MoverComp->GetUpdatedComponentTransform().GetLocation() + (CapsuleOffset);
		MoverComp->QueueInstantMovementEffect(TeleportEffect);
	}
	
	// Add offset to visual component as the base location has changed
	FTransform MoverVisualComponentOffset = MoverComp->GetBaseVisualComponentTransform();
	MoverVisualComponentOffset.SetLocation(MoverVisualComponentOffset.GetLocation() + VisualOffset);
	MoverComp->SetBaseVisualComponentTransform(MoverVisualComponentOffset);
}

void FCapsuleSizeModifier::ApplyMovementSettings(UMoverComponent* MoverComp)
{
}

void FCapsuleSizeModifier::RevertMovementSettings(UMoverComponent* MoverComp)
{
}