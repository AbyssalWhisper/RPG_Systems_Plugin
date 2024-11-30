// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/Footprint/RPG_FootprintComponent.h"

#include "RPG_FootprintActor.h"
#include "Components/DecalComponent.h"
#include "RPG_Systems/Footprint/RPG_FootprintData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
URPG_FootprintComponent::URPG_FootprintComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URPG_FootprintComponent::BeginPlay()
{
	Super::BeginPlay();

 
	SkeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

void URPG_FootprintComponent::TrySpawnFootprint(URPG_FootprintData* Footprint_,FName Socket)
{
	if (!UKismetSystemLibrary::IsDedicatedServer(this) && SkeletalMeshComponent)
	{

		FHitResult hit;
		FVector Start = SkeletalMeshComponent->GetSocketLocation(Socket);
		Start.Z += 50;
		FVector End = SkeletalMeshComponent->GetSocketLocation(Socket);
		End.Z -= 50;
		
		FCollisionQueryParams QueryParams;
		QueryParams.bReturnPhysicalMaterial = true;
		QueryParams.AddIgnoredActor(GetOwner());
		if (!Footprint && !Footprint_) return;
		if (GetWorld()->LineTraceSingleByChannel(hit, Start, End, ECollisionChannel::ECC_Visibility, QueryParams))
		{
			if (!hit.PhysMaterial.IsValid())return;

			UPhysicalMaterial* PhysMat = hit.PhysMaterial.Get();
			if(!PhysMat)return;
			if (Footprint_)
			{
				SpawnFootprint(Footprint_,PhysMat->SurfaceType, hit);
			}
			else
			{
				SpawnFootprint(Footprint,PhysMat->SurfaceType, hit);
			}
			
		}

	}
}

void URPG_FootprintComponent::SpawnFootprint(URPG_FootprintData* Footprint_,TEnumAsByte<EPhysicalSurface> Surface, const FHitResult& hit)
{
	if (!Footprint_->FootprintData.Contains(Surface))return;
	const FSTR_RPG_FootprintSettings& FootprintSettings = Footprint_->FootprintData.FindChecked(Surface);
	if (FootprintActors.Num()>= MaxFootprints)
	{
		ARPG_FootprintActor* ref = FootprintActors[0];
		FootprintActors.RemoveAt(0);
		FootprintActors.Add(ref);
		ref->DecalComponent->SetDecalMaterial(FootprintSettings.FootMaterial);
		ref->SetActorLocation(hit.Location);
		ref->SetActorScale3D(FVector(Footprint_->OffSetScale));
		ref->SetActorRotation(UKismetMathLibrary::MakeRotFromZX(hit.ImpactNormal,
			SkeletalMeshComponent->GetOwner()->GetActorForwardVector()));
	}
	else
	{
		ARPG_FootprintActor* FootprintActor;
		if (FootprintActorClass)
		{
			FootprintActor =  GetWorld()->SpawnActor<ARPG_FootprintActor>(FootprintActorClass, hit.Location, FRotator());
		}
		else
		{
			FootprintActor =  GetWorld()->SpawnActor<ARPG_FootprintActor>(ARPG_FootprintActor::StaticClass(), hit.Location, FRotator());
		}
		FootprintActor->OwnerActor = GetOwner();
		FootprintActor->DecalComponent->SetDecalMaterial(FootprintSettings.FootMaterial);
	
		FootprintActor->SetActorScale3D(FVector(Footprint_->OffSetScale));
		FootprintActor->SetActorRotation(UKismetMathLibrary::MakeRotFromZX(hit.ImpactNormal,
			SkeletalMeshComponent->GetOwner()->GetActorForwardVector()));
		 FootprintActors.Add(FootprintActor);
	}
	

	if (FootprintSettings.Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FootprintSettings.Sound, hit.ImpactPoint);

	}
	if (FootprintSettings.Particle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FootprintSettings.Particle, hit.ImpactPoint, FRotator::ZeroRotator);
	}
}



