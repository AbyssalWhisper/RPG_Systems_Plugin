// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RPG_Systems/Footprint/RPG_FootprintData.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "NiagaraSystem.h"
#include "Chaos/ChaosEngineInterface.h"
#include "RPG_FootprintComponent.generated.h"

class ACharacter;
class UNiagaraSystem;
class UMaterialInterface;
class USoundBase;
class ARPG_FootprintActor; 


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_SYSTEMS_API URPG_FootprintComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPG_FootprintComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	USkeletalMeshComponent* OwnerSkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URPG_FootprintData* Footprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARPG_FootprintActor> FootprintActorClass;
	UPROPERTY()
	TArray<ARPG_FootprintActor*> FootprintActors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxFootprints = 30;
	UFUNCTION(BlueprintCallable)
	void TrySpawnFootprint(URPG_FootprintData* Footprint_,FName Socket);
	void SpawnFootprint(URPG_FootprintData* Footprint_,TEnumAsByte<EPhysicalSurface> Surface, const FHitResult& hit);
		
};
