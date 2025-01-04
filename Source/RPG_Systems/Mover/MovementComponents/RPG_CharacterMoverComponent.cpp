// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG_Systems/Mover/MovementComponents/RPG_CharacterMoverComponent.h"

#include "BetterUtilitiesBPLibrary.h"
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

TObjectPtr<UBaseMovementMode> URPG_CharacterMoverComponent::GetCurrentMovementMode()
{
	if (bHasValidCachedState)
	{
		
		return MovementModes.Find(CachedLastSyncState.MovementMode)->Get();
	}
	return nullptr;
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
	if (bHasValidCachedState)
	{
		return CachedLastSyncState.MovementMode == DefaultModeNames::Swimming;
	}

	return false;
}
