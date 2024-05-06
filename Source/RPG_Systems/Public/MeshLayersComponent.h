// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "MeshLayersComponent.generated.h"

USTRUCT(BlueprintType)
struct FMeshLayerPreset
{
	GENERATED_BODY()
	public:
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName Socket;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TEnumAsByte<ECollisionEnabled::Type> Collision = ECollisionEnabled::Type::NoCollision;
	FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FVector LocationOffSet;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool UsePoseMaster = false;
	FMeshLayerPreset()
	{
	}
};

USTRUCT(BlueprintType)
struct FMeshLayers
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TMap<FGameplayTag,UMeshComponent*> MeshLayersList;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_SYSTEMS_API UMeshLayersComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeshLayersComponent();
	UPROPERTY()
	TMap<UMeshComponent*,FMeshLayers> Meshs;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* AddStaticMeshLayer(UMeshComponent* OwnerMesh,UStaticMesh* Mesh,FGameplayTag LayerTag,FMeshLayerPreset Preset);
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* AddSkeletalMeshLayer(UMeshComponent* OwnerMesh, USkeletalMesh* Mesh, FGameplayTag LayerTag,
	                                        FMeshLayerPreset Preset);
	UFUNCTION(BlueprintCallable)
	void RemoveMeshLayer(UMeshComponent* OwnerMesh,FGameplayTag LayerTag);
	
		
};
