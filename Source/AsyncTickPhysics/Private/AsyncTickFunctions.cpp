#include "AsyncTickFunctions.h"

#include "Chaos/Capsule.h"
#include "Chaos/ImplicitObjectScaled.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/BodyInstance.h"

void UAsyncTickFunctions::ATP_AddForce(UPrimitiveComponent* Component, FVector Force, bool bAccelChange, FName BoneName)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component, BoneName))
	{
		if (bAccelChange)
		{
			const float RigidMass = RigidHandle->M();
			const Chaos::FVec3 Acceleration = Force * RigidMass;
			RigidHandle->AddForce(Acceleration, false);
		}
		else
		{
			RigidHandle->AddForce(Force, false);
		}
	}
}

void UAsyncTickFunctions::ATP_AddForceAtPosition(UPrimitiveComponent* Component, FVector Position, FVector Force,
                                                 FName BoneName)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component, BoneName))
	{
		const Chaos::FVec3 WorldCOM = Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(RigidHandle);
		const Chaos::FVec3 WorldTorque = Chaos::FVec3::CrossProduct(Position - WorldCOM, Force);
		RigidHandle->AddForce(Force, false);
		RigidHandle->AddTorque(WorldTorque, false);
	}
}

void UAsyncTickFunctions::ATP_AddTorque(UPrimitiveComponent* Component, FVector Torque, bool bAccelChange,
                                        FName BoneName)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component, BoneName))
	{
		if (bAccelChange)
		{
			RigidHandle->AddTorque(Chaos::FParticleUtilitiesXR::GetWorldInertia(RigidHandle) * Torque, false);
		}
		else
		{
			RigidHandle->AddTorque(Torque, false);
		}
	}
}

void UAsyncTickFunctions::ATP_AddImpulse(UPrimitiveComponent* Component, FVector Impulse, bool bVelChange,
                                         FName BoneName)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component, BoneName))
	{
		if (bVelChange)
		{
			RigidHandle->SetLinearImpulse(RigidHandle->LinearImpulse() + RigidHandle->M() * Impulse, false);
		}
		else
		{
			RigidHandle->SetLinearImpulse(RigidHandle->LinearImpulse() + Impulse, false);
		}
	}
}

void UAsyncTickFunctions::ATP_AddImpulseAtPosition(UPrimitiveComponent* Component, FVector Position, FVector Impulse,
                                                   FName BoneName)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component, BoneName))
	{
		const Chaos::FVec3 WorldCOM = Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(RigidHandle);
		const Chaos::FVec3 AngularImpulse = Chaos::FVec3::CrossProduct(Position - WorldCOM, Impulse);
		RigidHandle->SetLinearImpulse(RigidHandle->LinearImpulse() + Impulse, false);
		RigidHandle->SetAngularImpulse(RigidHandle->AngularImpulse() + AngularImpulse, false);
	}
}

void UAsyncTickFunctions::ATP_AddAngularImpulseInRadians(UPrimitiveComponent* Component, FVector Torque,
                                                         bool bVelChange, FName BoneName)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component, BoneName))
	{
		if (bVelChange)
		{
			const Chaos::FMatrix33 WorldI = Chaos::FParticleUtilitiesXR::GetWorldInertia(RigidHandle);
			RigidHandle->SetAngularImpulse(RigidHandle->AngularImpulse() + (WorldI * Torque), false);
		}
		else
		{
			RigidHandle->SetAngularImpulse(RigidHandle->AngularImpulse() + Torque, false);
		}
	}
}

void UAsyncTickFunctions::ATP_AddAngularImpulseInDegrees(UPrimitiveComponent* Component, FVector Torque,
                                                         bool bVelChange, FName BoneName)
{
	ATP_AddAngularImpulseInRadians(Component, FMath::DegreesToRadians(Torque), bVelChange, BoneName);
}

FTransform UAsyncTickFunctions::ATP_GetTransform(UPrimitiveComponent* Component)
{
	if (const Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component))
	{
		const Chaos::FRigidTransform3 WorldCOM = Chaos::FParticleUtilitiesGT::GetActorWorldTransform(RigidHandle);
		return WorldCOM;
	}
	return Component ? Component->GetComponentTransform() : FTransform();
}



FVector UAsyncTickFunctions::ATP_GetLinearVelocity(UPrimitiveComponent* Component, FName BoneName)
{
	if (const Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component, BoneName))
	{
		return RigidHandle->V();
	}
	return FVector::ZeroVector;
}

FVector UAsyncTickFunctions::ATP_GetCoMPosition(UPrimitiveComponent* Component)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component))
	{
		if (ensure(RigidHandle->CanTreatAsKinematic()))
		{
			const bool bIsRigid = RigidHandle->CanTreatAsRigid();
			return bIsRigid
				       ? Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(RigidHandle)
				       : static_cast<Chaos::FVec3>(Chaos::FParticleUtilitiesGT::GetActorWorldTransform(RigidHandle).
					       GetTranslation());
		}
	}
	return FVector::ZeroVector;
}


FVector UAsyncTickFunctions::ATP_GetAngularVelocity(UPrimitiveComponent* Component, FName BoneName)
{
	if (const Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component, BoneName))
	{
		return RigidHandle->W();
	}
	return FVector::ZeroVector;
}

void UAsyncTickFunctions::ATP_SetLinearVelocity(UPrimitiveComponent* Component, FVector Velocity, bool bAddToCurrent,
                                                FName BoneName)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component, BoneName))
	{
		if (bAddToCurrent)
		{
			RigidHandle->SetV(RigidHandle->V() + Velocity);
		}
		else
		{
			RigidHandle->SetV(Velocity);
		}
	}
}

void UAsyncTickFunctions::ATP_SetAngularVelocityInRadians(UPrimitiveComponent* Component, FVector AngVelocity,
                                                          bool bAddToCurrent, FName BoneName)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component, BoneName))
	{
		if (bAddToCurrent)
		{
			RigidHandle->SetW(RigidHandle->W() + AngVelocity);
		}
		else
		{
			RigidHandle->SetW(AngVelocity);
		}
	}
}

void UAsyncTickFunctions::ATP_SetAngularVelocityInDegrees(UPrimitiveComponent* Component, FVector AngVelocity,
                                                          bool bAddToCurrent, FName BoneName)
{
	ATP_SetAngularVelocityInRadians(Component, FMath::DegreesToRadians(AngVelocity), bAddToCurrent, BoneName);
}

void UAsyncTickFunctions::ATP_SetWorldLocation(USceneComponent* Component, FVector Location)
{
	if (!Component)
	{
		return;
	}

	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component))
	{
		if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(PrimitiveComponent))
		{
			const Chaos::FVec3 P = Location - RigidHandle->R().RotateVector(RigidHandle->CenterOfMass());
			RigidHandle->SetX(P);
		}
	}
	else
	{
		Component->SetWorldLocation(Location);
	}
}

void UAsyncTickFunctions::ATP_SetWorldRotation(UPrimitiveComponent* Component, FRotator Rotation)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component))
	{
		const Chaos::FRotation3 Q = Rotation.Quaternion() * RigidHandle->RotationOfMass().Inverse();
		RigidHandle->SetR(Q);
	}
}

void UAsyncTickFunctions::ATP_SetWorldLocationAndRotation(UPrimitiveComponent* Component, FVector Location,
                                                          FRotator Rotation)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component))
	{
		const Chaos::FRotation3 Q = Rotation.Quaternion() * RigidHandle->RotationOfMass().Inverse();
		const Chaos::FVec3 P = Location - Q.RotateVector(RigidHandle->CenterOfMass());
		RigidHandle->SetR(Q);
		RigidHandle->SetX(P);
	}
}

FVector UAsyncTickFunctions::ATP_GetLinearVelocityAtPoint(UPrimitiveComponent* Component, FVector Point, FName BoneName)
{
	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Component, BoneName))
	{
		if (ensure(RigidHandle->CanTreatAsKinematic()))
		{
			const bool bIsRigid = RigidHandle->CanTreatAsRigid();
			const Chaos::FVec3 COM = bIsRigid
				                         ? Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(RigidHandle)
				                         : static_cast<Chaos::FVec3>(
					                         Chaos::FParticleUtilitiesGT::GetActorWorldTransform(RigidHandle).
					                         GetTranslation());
			const Chaos::FVec3 Diff = Point - COM;
			return RigidHandle->V() - Chaos::FVec3::CrossProduct(Diff, RigidHandle->W());
		}
	}
	return FVector::ZeroVector;
}

void UAsyncTickFunctions::DebugDrawParticleGeometry(UPrimitiveComponent* Component, float Duration, float Thickness)
{
	if (!Component || !Component->GetWorld())
	{
		//UE_LOG(LogTemp, Error, TEXT("Component ou World é NULL!"));
		return;
	}

	FBodyInstance* BodyInstance = Component->GetBodyInstance();
	if (!BodyInstance)
	{
		//UE_LOG(LogTemp, Error, TEXT("BodyInstance é NULL!"));
		return;
	}

	FPhysicsActorHandle& ActorHandle = BodyInstance->GetPhysicsActorHandle();
	if (!ActorHandle)
	{
		//UE_LOG(LogTemp, Error, TEXT("ActorHandle é NULL!"));
		return;
	}

	Chaos::FRigidBodyHandle_Internal* ParticleHandle = ActorHandle->GetPhysicsThreadAPI();
	if (!ParticleHandle)
	{
		//UE_LOG(LogTemp, Error, TEXT("ParticleHandle é NULL!"));
		return;
	}

	const Chaos::FImplicitObject* Geometry = ParticleHandle->GetGeometry();
	if (!Geometry)
	{
		//UE_LOG(LogTemp, Error, TEXT("Geometry é NULL!"));
		return;
	}

	// Obter posição e rotação da partícula
	const Chaos::FVec3& Position = ParticleHandle->X();
	const Chaos::FRotation3& Rotation = ParticleHandle->R();
	FVector ParticleLocation(Position.X, Position.Y, Position.Z);
	FQuat WorldRotation(Rotation.X, Rotation.Y, Rotation.Z, Rotation.W);

	UWorld* World = Component->GetWorld();
	FColor DebugColor = FColor::Green;

	// Obter o offset do centro de massa (center of mass)
	const Chaos::FVec3& CenterOfMass = ParticleHandle->CenterOfMass();
	FVector COMOffset(CenterOfMass.X, CenterOfMass.Y, CenterOfMass.Z);
	
	// A localização real da geometria é: posição da partícula + offset rotacionado
	FVector GeometryCenter = ParticleLocation + WorldRotation.RotateVector(COMOffset);

	//UE_LOG(LogTemp, Warning, TEXT("Particle Location: %s"), *ParticleLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("COM Offset: %s"), *COMOffset.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Geometry Center: %s"), *GeometryCenter.ToString());

	// Tentar desenhar cápsula
	const Chaos::FCapsule* Capsule = Geometry->GetObject<Chaos::FCapsule>();
	if (Capsule)
	{
		float Radius = Capsule->GetRadius();
		float HalfHeight = Capsule->GetHeight() * 0.5f;
		
		// Obter o centro da cápsula (pode ter offset adicional)
		const Chaos::FVec3& CapsuleCenter = Capsule->GetCenterf();
		FVector CapsuleOffset(CapsuleCenter.X, CapsuleCenter.Y, CapsuleCenter.Z);
		FVector FinalLocation = ParticleLocation + WorldRotation.RotateVector(CapsuleOffset);
		
		DrawDebugCapsule(World, FinalLocation, HalfHeight + Radius, Radius, WorldRotation, DebugColor, false, Duration, 0, Thickness);
		DrawDebugSphere(World, ParticleLocation, 10.0f, 8, FColor::Blue, false, Duration, 0, Thickness); // Particle origin
		DrawDebugSphere(World, ParticleLocation + WorldRotation.RotateVector(CapsuleOffset), 5.0f, 8, FColor::Red, false, Duration, 0, Thickness); // Capsule center
		
		
		//UE_LOG(LogTemp, Warning, TEXT("Debug Draw: Cápsula - Raio: %f, HalfHeight: %f, CapsuleOffset: %s"), Radius, HalfHeight, *CapsuleOffset.ToString());
		return;
	}

	// Tentar geometria scaled
	if (const Chaos::TImplicitObjectScaled<Chaos::FCapsule>* ScaledCapsule = Geometry->GetObject<Chaos::TImplicitObjectScaled<Chaos::FCapsule>>())
	{
		const Chaos::FCapsule* InnerCapsule = ScaledCapsule->GetUnscaledObject();
		if (InnerCapsule)
		{
			FVector Scale = ScaledCapsule->GetScale();
			float Radius = InnerCapsule->GetRadiusf() * Scale.X;
			float HalfHeight = (InnerCapsule->GetHeightf() * Scale.Z) * 0.5f;
			
			// Obter o centro da cápsula com scale aplicado
			const Chaos::FVec3& CapsuleCenter = InnerCapsule->GetCenterf();
			FVector CapsuleOffset(CapsuleCenter.X * Scale.X, CapsuleCenter.Y * Scale.Y, CapsuleCenter.Z * Scale.Z);
			FVector FinalLocation = ParticleLocation + WorldRotation.RotateVector(CapsuleOffset);
			
			DrawDebugCapsule(World, FinalLocation, HalfHeight + Radius, Radius, WorldRotation, DebugColor, false, Duration, 0, Thickness);
			DrawDebugSphere(World, ParticleLocation, 10.0f, 8, FColor::Blue, false, Duration, 0, Thickness); // Particle origin
			
			//UE_LOG(LogTemp, Warning, TEXT("Debug Draw: Cápsula Scaled - Raio: %f, HalfHeight: %f, Scale: %s, CapsuleOffset: %s"), Radius, HalfHeight, *Scale.ToString(), *CapsuleOffset.ToString());
			return;
		}
	}

	// Fallback: desenhar uma esfera
	DrawDebugSphere(World, GeometryCenter, 10.0f, 12, FColor::Red, false, Duration, 0, Thickness);
	//UE_LOG(LogTemp, Warning, TEXT("Debug Draw: Geometria desconhecida - desenhando esfera"));
}


Chaos::FRigidBodyHandle_Internal* UAsyncTickFunctions::GetInternalHandle(UPrimitiveComponent* Component, FName BoneName)
{
	if (IsValid(Component))
	{
		if (const FBodyInstance* BodyInstance = Component->GetBodyInstance(BoneName))
		{
			if (const auto Handle = BodyInstance->ActorHandle)
			{
				if (Chaos::FRigidBodyHandle_Internal* RigidHandle = Handle->GetPhysicsThreadAPI())
				{
					return RigidHandle;
				}
			}
		}
	}
	return nullptr;
}
