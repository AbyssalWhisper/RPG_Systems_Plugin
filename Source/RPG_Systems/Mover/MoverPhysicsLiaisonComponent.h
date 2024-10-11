// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Backends/MoverNetworkPhysicsLiaison.h"
#include "MoverPhysicsLiaisonComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RPG_SYSTEMS_API UMoverPhysicsLiaisonComponent : public UMoverNetworkPhysicsLiaisonComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMoverPhysicsLiaisonComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
#if WITH_EDITOR
	virtual EDataValidationResult ValidateData(FDataValidationContext& Context,
		const UMoverComponent& ValidationMoverComp) const override;
#endif
	
};
