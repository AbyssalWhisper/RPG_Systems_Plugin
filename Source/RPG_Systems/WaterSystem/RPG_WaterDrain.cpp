


#include "RPG_Systems/WaterSystem/RPG_WaterDrain.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Components/PostProcessComponent.h"
#include "GameFramework/PhysicsVolume.h"
#include "Components/BrushComponent.h"

// Sets default values
ARPG_WaterDrain::ARPG_WaterDrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
 
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(FName("RootScene"));
	RootComponent = Root;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	bReplicates = true;
	SetReplicateMovement(true);
	BoxComponent->SetupAttachment(Root);
	BoxComponent->SetBoxExtent(FVector(100), true);
	WaterMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("WaterMesh"));
	WaterMesh->SetupAttachment(RootComponent);
	WaterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(FName("PostProcess"));
	PostProcess->bUnbound = false;
	PostProcess->SetupAttachment(WaterMesh);
	PostProcess->bUseAttachParentBound = true;
}

// Called when the game starts or when spawned
void ARPG_WaterDrain::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		FVector SpawnLocation = FVector(0.0f, 0.0f, 0.0f);

		// Rotação do PhysicsVolume (altere conforme necessário)
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

		PhysicsVolume = GetWorld()->SpawnActor<APhysicsVolume>(APhysicsVolume::StaticClass(),SpawnLocation, SpawnRotation);
		if (PhysicsVolume)
		{
			if (!PhysicsVolume->GetBrushComponent()->BrushBodySetup)
			{  
				PhysicsVolume->GetBrushComponent()->BrushBodySetup= BoxComponent->GetBodySetup();
				PhysicsVolume->GetBrushComponent()->SetMobility(EComponentMobility::Movable);
				//PhysicsVolume->GetBrushComponent()->BrushBodySetup->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseSimpleAsComplex;

				PhysicsVolume->bWaterVolume = true;
				PhysicsVolume->TerminalVelocity = TerminalVelocity;
				PhysicsVolume->Priority = Priority;
				PhysicsVolume->FluidFriction = FluidFriction; 
				PhysicsVolume->GetBrushComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
				PhysicsVolume->GetBrushComponent()->SetCollisionResponseToChannel(WaterCollisionChannel,ECR_Block); 
				
			} 
		}
	}
	UpdateWater(); 
	if (UpdateWaterByTime)
	{
		GetWorldTimerManager().SetTimer(TimeHandle, this, &ARPG_WaterDrain::WaterUpdateTick, 1, true);
	}
}

void ARPG_WaterDrain::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (PhysicsVolume)
	{
		PhysicsVolume->Destroy();
	}
}

void ARPG_WaterDrain::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ARPG_WaterDrain, CurrentWater, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME(ARPG_WaterDrain, MaxWater);
	DOREPLIFETIME(ARPG_WaterDrain, WaterValueCanChange);
	
}
 
void ARPG_WaterDrain::UpdateWater()
{
	float Alpha = CurrentWater / MaxWater;

	FVector TargetLocation = GetActorLocation() + (this->GetActorUpVector() * GetActorScale3D().Z * (Alpha * 100));
	BoxComponent->SetRelativeScale3D(FVector(1.f, 1.f, Alpha));
	BoxComponent->SetWorldLocation(TargetLocation);
	FVector Scale = this->GetActorScale3D();
	BoxComponent->SetWorldRotation(GetActorRotation());
	PostProcess->SetRelativeScale3D(FVector(1.f, 1.f, Alpha));
	PostProcess->SetWorldLocation(TargetLocation);
	PostProcess->SetWorldRotation(GetActorRotation());
	if (PhysicsVolume)
	{
		PhysicsVolume->SetActorScale3D(FVector(GetActorScale3D().X, GetActorScale3D().Y, GetActorScale3D().Z*Alpha));
		PhysicsVolume->SetActorLocation(TargetLocation);
		PhysicsVolume->SetActorRotation(GetActorRotation()); 

	}
	
	WaterMesh->SetRelativeScale3D(FVector(1.f, 1.f, Alpha));

	//BoxComponent->SetWorldScale3D(FVector(Scale.X, Scale.Y,Alpha));
	//BoxComponent->SetWorldScale3D(BoxComponent->GetComponentScale().X, BoxComponent->GetComponentScale().Y, BoxComponent->GetComponentScale().Z)
	for (int i = 0; i < Actors.Num(); i++)
	{
		if (Actors[i].Actor)
		{
			Actors[i].Actor->SetActorLocation(TargetLocation + Actors[i].OffSetLocation);
			if (Actors[i].UpdateScale)
			{
				Actors[i].Actor->SetActorScale3D(FVector(Scale.X, Scale.Y, Scale.Z * Alpha) * Actors[i].OffSetScale);
			}
			if (Actors[i].UpdateRotation)
			{
				Actors[i].Actor->SetActorRotation(GetActorRotation());
			}
			//Actors[i].Actor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}
	}
	 
	FVector Height = GetActorLocation() + (this->GetActorUpVector() * GetActorScale3D().Z * (Alpha * 200));
	WaterHeight = Height.Z;
	OnWaterHeightChange(WaterHeight);
}

void ARPG_WaterDrain::OnConstruction(const FTransform& Transform)
{
	CurrentWater = FMath::Clamp(CurrentWater, 0, MaxWater);
	UpdateWater();
#if WITH_EDITOR
	if (ActorWorldGuid.IsValid())return;
	ActorWorldGuid = ActorGuid;
#endif
}

void ARPG_WaterDrain::AddWater(float Water)
{
	if (WaterValueCanChange)
	{
		CurrentWater = FMath::Clamp(CurrentWater + Water, 0, MaxWater);
		OnRep_CurrentWater();
		OnWaterValueChange(CurrentWater);
	}
	
}

void ARPG_WaterDrain::SetWaterPercent(float Percent)
{
	if (WaterValueCanChange)
	{
		CurrentWater = FMath::Lerp(0, MaxWater, Percent);
		OnRep_CurrentWater();
		OnWaterValueChange(CurrentWater);
	}
}
void ARPG_WaterDrain::WaterUpdateTick()
{
	if (WaterValueCanChange)
	{
		AddWater(GetWaterUpdateValue());
	}
}


void ARPG_WaterDrain::OnRep_CurrentWater()
{
	UpdateWater();
}

float ARPG_WaterDrain::GetWaterUpdateValue_Implementation()
{
	return 0.0f;
}

void ARPG_WaterDrain::OnWaterHeightChange_Implementation(float Value)
{
}

void ARPG_WaterDrain::OnWaterValueChange_Implementation(float Value)
{
}

FGuid ARPG_WaterDrain::GetGuid()
{
	return ActorWorldGuid;
}

void ARPG_WaterDrain::InitGuid()
{
	if (ActorWorldGuid.IsValid())return;
	ActorWorldGuid = FGuid::NewGuid();
}

bool ARPG_WaterDrain::GetIsMapSpawnActor()
{
	return bNetStartup;
}