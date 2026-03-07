// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicCollisionComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ADynamicCollisionComponent::ADynamicCollisionComponent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionType = ECollisionComponentType::Box;
	CollisionComponent = nullptr;
}

void ADynamicCollisionComponent::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	CreateCollisionComponent();
}

void ADynamicCollisionComponent::CreateCollisionComponent()
{
	// Store previous collision settings before destroying component
	ECollisionEnabled::Type PreviousCollisionEnabled = ECollisionEnabled::QueryAndPhysics;
	FCollisionResponseContainer PreviousCollisionResponses;
	bool bHadPreviousComponent = false;

	if (CollisionComponent)
	{
		// Save previous settings
		PreviousCollisionEnabled = CollisionComponent->GetCollisionEnabled();
		PreviousCollisionResponses = CollisionComponent->GetCollisionResponseToChannels();
		bHadPreviousComponent = true;

		// Remove existing collision component
		CollisionComponent->DestroyComponent();
		CollisionComponent = nullptr;
	}

	// Create new collision component based on selected type
	switch (CollisionType)
	{
	case ECollisionComponentType::Box:
		{
			UBoxComponent* BoxComp = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("BoxCollision"));
			if (BoxComp)
			{
				BoxComp->RegisterComponent();
				BoxComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				BoxComp->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
				CollisionComponent = BoxComp;
			}
		}
		break;

	case ECollisionComponentType::Sphere:
		{
			USphereComponent* SphereComp = NewObject<USphereComponent>(this, USphereComponent::StaticClass(), TEXT("SphereCollision"));
			if (SphereComp)
			{
				SphereComp->RegisterComponent();
				SphereComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				SphereComp->SetSphereRadius(100.0f);
				CollisionComponent = SphereComp;
			}
		}
		break;

	case ECollisionComponentType::Capsule:
		{
			UCapsuleComponent* CapsuleComp = NewObject<UCapsuleComponent>(this, UCapsuleComponent::StaticClass(), TEXT("CapsuleCollision"));
			if (CapsuleComp)
			{
				CapsuleComp->RegisterComponent();
				CapsuleComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				CapsuleComp->SetCapsuleSize(50.0f, 100.0f);
				CollisionComponent = CapsuleComp;
			}
		}
		break;
	}

	// Apply previous collision settings to new component
	if (CollisionComponent && bHadPreviousComponent)
	{
		CollisionComponent->SetCollisionEnabled(PreviousCollisionEnabled);
		CollisionComponent->SetCollisionResponseToChannels(PreviousCollisionResponses);
	}
}

// Called when the game starts or when spawned
void ADynamicCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADynamicCollisionComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

