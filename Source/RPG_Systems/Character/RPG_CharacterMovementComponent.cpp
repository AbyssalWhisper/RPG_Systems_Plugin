// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Character/RPG_CharacterMovementComponent.h"
#include "RPG_Systems/Character/RPG_BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"


void URPG_CharacterMovementComponent::BeginPlay()
{
	OwnerCharacter = Cast<ARPG_BaseCharacter>(GetOwner());
}

float URPG_CharacterMovementComponent::GetMaxSpeed() const
{
	//
	return Super::GetMaxSpeed();
	/*
	if (!OwnerCharacter)
	{
		return Super::GetMaxSpeed();
	}

	if (!OwnerCharacter->GetAbilitySystemComponent() || OwnerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(OwnerCharacter->StunTag))
	{
		return 0;
	}*/
	//return OwnerCharacter->GetMoveSpeed();
}
/*
void URPG_CharacterMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TaskTest.IsValid())
	{
		if (TaskTest.IsCompleted())
		{
			const UE::Tasks::ETaskPriority TaskPriority = UE::Tasks::ETaskPriority::High;
			TaskTest = UE::Tasks::Launch(TEXT("Calculate test"), [this] {
				float DT = 0.0166666666666667;
				FVector InputVector = FVector::ZeroVector;
				bool bUsingAsyncTick = (CharacterMovementCVars::AsyncCharacterMovement == 1) && IsAsyncCallbackRegistered();
				if (!bUsingAsyncTick)
				{
					// Do not consume input if simulating asynchronously, we will consume input when filling out async inputs.
					InputVector = ConsumeInputVector();
				}

				if (!HasValidData() || ShouldSkipUpdate(DT))
				{
					return;
				}

				 

				// Super tick may destroy/invalidate CharacterOwner or UpdatedComponent, so we need to re-check.
				if (!HasValidData())
				{
					return;
				}

				if (bUsingAsyncTick)
				{
					check(CharacterOwner && CharacterOwner->GetMesh());
					USkeletalMeshComponent* CharacterMesh = CharacterOwner->GetMesh();
					if (CharacterMesh->ShouldTickPose())
					{
						const bool bWasPlayingRootMotion = CharacterOwner->IsPlayingRootMotion();

						CharacterMesh->TickPose(DT, true);
						// We are simulating character movement on physics thread, do not tick movement.
						const bool bIsPlayingRootMotion = CharacterOwner->IsPlayingRootMotion();
						if (bIsPlayingRootMotion || bWasPlayingRootMotion)
						{
							FRootMotionMovementParams RootMotion = CharacterMesh->ConsumeRootMotion();
							if (RootMotion.bHasRootMotion)
							{
								RootMotion.ScaleRootMotionTranslation(CharacterOwner->GetAnimRootMotionTranslationScale());
								RootMotionParams.Accumulate(RootMotion);
							}
						}
					}

					AccumulateRootMotionForAsync(DT, AsyncRootMotion);

					return;
				}

				// See if we fell out of the world.
				const bool bIsSimulatingPhysics = UpdatedComponent->IsSimulatingPhysics();
				if (CharacterOwner->GetLocalRole() == ROLE_Authority && (!bCheatFlying || bIsSimulatingPhysics) && !CharacterOwner->CheckStillInWorld())
				{
					return;
				}

				// We don't update if simulating physics (eg ragdolls).
				if (bIsSimulatingPhysics)
				{
					// Update camera to ensure client gets updates even when physics move him far away from point where simulation started
					if (CharacterOwner->GetLocalRole() == ROLE_AutonomousProxy && IsNetMode(NM_Client))
					{
						MarkForClientCameraUpdate();
					}

					ClearAccumulatedForces();
					return;
				}

				AvoidanceLockTimer -= DT;

				if (CharacterOwner->GetLocalRole() > ROLE_SimulatedProxy)
				{
					 

					// If we are a client we might have received an update from the server.
					const bool bIsClient = (CharacterOwner->GetLocalRole() == ROLE_AutonomousProxy && IsNetMode(NM_Client));
					if (bIsClient)
					{
						FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character();
						if (ClientData && ClientData->bUpdatePosition)
						{
							ClientUpdatePositionAfterServerUpdate();
						}
					}

					// Allow root motion to move characters that have no controller.
					if (CharacterOwner->IsLocallyControlled() || (!CharacterOwner->Controller && bRunPhysicsWithNoController) || (!CharacterOwner->Controller && CharacterOwner->IsPlayingRootMotion()))
					{
						ControlledCharacterMove(InputVector, DT);
					}
					else if (CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy)
					{
						// Server ticking for remote client.
						// Between net updates from the client we need to update position if based on another object,
						// otherwise the object will move on intermediate frames and we won't follow it.
						MaybeUpdateBasedMovement(DT);
						MaybeSaveBaseLocation();


					}
				}


				});
		}
	}
	else
	{
		const UE::Tasks::ETaskPriority TaskPriority = UE::Tasks::ETaskPriority::High;
		TaskTest = UE::Tasks::Launch(TEXT("Calculate test"), [this] {
			float DT = 0.0166666666666667;
			FVector InputVector = FVector::ZeroVector;
			bool bUsingAsyncTick = (CharacterMovementCVars::AsyncCharacterMovement == 1) && IsAsyncCallbackRegistered();
			if (!bUsingAsyncTick)
			{
				// Do not consume input if simulating asynchronously, we will consume input when filling out async inputs.
				InputVector = ConsumeInputVector();
			}

			if (!HasValidData() || ShouldSkipUpdate(DT))
			{
				return;
			}



			// Super tick may destroy/invalidate CharacterOwner or UpdatedComponent, so we need to re-check.
			if (!HasValidData())
			{
				return;
			}

			if (bUsingAsyncTick)
			{
				check(CharacterOwner && CharacterOwner->GetMesh());
				USkeletalMeshComponent* CharacterMesh = CharacterOwner->GetMesh();
				if (CharacterMesh->ShouldTickPose())
				{
					const bool bWasPlayingRootMotion = CharacterOwner->IsPlayingRootMotion();

					CharacterMesh->TickPose(DT, true);
					// We are simulating character movement on physics thread, do not tick movement.
					const bool bIsPlayingRootMotion = CharacterOwner->IsPlayingRootMotion();
					if (bIsPlayingRootMotion || bWasPlayingRootMotion)
					{
						FRootMotionMovementParams RootMotion = CharacterMesh->ConsumeRootMotion();
						if (RootMotion.bHasRootMotion)
						{
							RootMotion.ScaleRootMotionTranslation(CharacterOwner->GetAnimRootMotionTranslationScale());
							RootMotionParams.Accumulate(RootMotion);
						}
					}
				}

				AccumulateRootMotionForAsync(DT, AsyncRootMotion);

				return;
			}

			// See if we fell out of the world.
			const bool bIsSimulatingPhysics = UpdatedComponent->IsSimulatingPhysics();
			if (CharacterOwner->GetLocalRole() == ROLE_Authority && (!bCheatFlying || bIsSimulatingPhysics) && !CharacterOwner->CheckStillInWorld())
			{
				return;
			}

			// We don't update if simulating physics (eg ragdolls).
			if (bIsSimulatingPhysics)
			{
				// Update camera to ensure client gets updates even when physics move him far away from point where simulation started
				if (CharacterOwner->GetLocalRole() == ROLE_AutonomousProxy && IsNetMode(NM_Client))
				{
					MarkForClientCameraUpdate();
				}

				ClearAccumulatedForces();
				return;
			}

			AvoidanceLockTimer -= DT;

			if (CharacterOwner->GetLocalRole() > ROLE_SimulatedProxy)
			{


				// If we are a client we might have received an update from the server.
				const bool bIsClient = (CharacterOwner->GetLocalRole() == ROLE_AutonomousProxy && IsNetMode(NM_Client));
				if (bIsClient)
				{
					FNetworkPredictionData_Client_Character* ClientData = GetPredictionData_Client_Character();
					if (ClientData && ClientData->bUpdatePosition)
					{
						ClientUpdatePositionAfterServerUpdate();
					}
				}

				// Allow root motion to move characters that have no controller.
				if (CharacterOwner->IsLocallyControlled() || (!CharacterOwner->Controller && bRunPhysicsWithNoController) || (!CharacterOwner->Controller && CharacterOwner->IsPlayingRootMotion()))
				{
					ControlledCharacterMove(InputVector, DT);
				}
				else if (CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy)
				{
					// Server ticking for remote client.
					// Between net updates from the client we need to update position if based on another object,
					// otherwise the object will move on intermediate frames and we won't follow it.
					MaybeUpdateBasedMovement(DT);
					MaybeSaveBaseLocation();


				}
			}
			});
	}
	
}*/
