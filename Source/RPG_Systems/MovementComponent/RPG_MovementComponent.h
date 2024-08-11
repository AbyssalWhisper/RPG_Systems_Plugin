// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RPG_MovementComponent.generated.h"

class URPG_BaseMovementMode;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_SYSTEMS_API URPG_MovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPG_MovementComponent();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector DirectionInput;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector LastDirectionInput;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayTag StartMovementMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	URPG_BaseMovementMode* CurrentMovementMode;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FGameplayTag, URPG_BaseMovementMode*> MovementModes;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void AddInput(FVector Input);
};
