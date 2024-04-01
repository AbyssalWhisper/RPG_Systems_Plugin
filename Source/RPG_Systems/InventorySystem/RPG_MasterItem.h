// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPG_Systems/General_Interfaces/RPG_Interact_Interface.h"
#include "RPG_Systems/InventorySystem/RPG_ItemStruct.h"
#include "RPG_MasterItem.generated.h"

UCLASS()
class RPG_SYSTEMS_API ARPG_MasterItem : public AActor , public IRPG_Interact_Interface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPG_MasterItem();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
	FSTR_RPG_ItemSlot Item;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Interact(APlayerController* PlayerController);
		void Interact_Implementation(APlayerController* PlayerController);
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
