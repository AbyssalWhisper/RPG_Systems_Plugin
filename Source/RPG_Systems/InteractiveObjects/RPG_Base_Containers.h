// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Misc/Guid.h"
#include "RPG_Systems/General_Interfaces/RPG_Interact_Interface.h"
#include "Engine/DataTable.h"
#include "RPG_Base_Containers.generated.h"

class URPG_InventoryComponent;
UCLASS()
class RPG_SYSTEMS_API ARPG_Base_Containers : public AActor , public IRPG_Interact_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPG_Base_Containers();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;
	UFUNCTION(BlueprintCallable)
	virtual void InitGuid();
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle LootList;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, NonPIEDuplicateTransient, NonTransactional)
		FGuid ActorWorldGuid;
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
		USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
		URPG_InventoryComponent* ContainerInventory;
	UFUNCTION(BlueprintCallable)
		FGuid GetGuid();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Interact(APlayerController* PlayerController);
	void Interact_Implementation(APlayerController* PlayerController);
	virtual void OnConstruction(const FTransform& Transform) override;

};
