// Fill out your copyright notice in the Description page of Project Settings.


#include "MoverPhysicsLiaisonComponent.h"


// Sets default values for this component's properties
UMoverPhysicsLiaisonComponent::UMoverPhysicsLiaisonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(false);
	// ...
}


// Called when the game starts
void UMoverPhysicsLiaisonComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMoverPhysicsLiaisonComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

EDataValidationResult UMoverPhysicsLiaisonComponent::ValidateData(FDataValidationContext& Context,
	const UMoverComponent& ValidationMoverComp) const
{
	return EDataValidationResult::Valid;
}

