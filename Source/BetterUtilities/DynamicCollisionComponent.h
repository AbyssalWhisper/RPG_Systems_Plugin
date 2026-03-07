// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ShapeComponent.h"

#include "DynamicCollisionComponent.generated.h"

UENUM(BlueprintType)
enum class ECollisionComponentType : uint8
{
	Box UMETA(DisplayName = "Box Collider"),
	Sphere UMETA(DisplayName = "Sphere Collider"),
	Capsule UMETA(DisplayName = "Capsule Collider")
};

UCLASS()
class BETTERUTILITIES_API ADynamicCollisionComponent : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADynamicCollisionComponent();

	// Construction Script
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	ECollisionComponentType CollisionType;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Collision")
	UShapeComponent* CollisionComponent;

	void CreateCollisionComponent();
};
