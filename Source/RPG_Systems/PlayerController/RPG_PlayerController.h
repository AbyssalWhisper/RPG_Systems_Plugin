// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPG_Systems/General_Interfaces/RPG_PlayerInterface.h"
#include "RPG_Systems/InventorySystem/RPG_InventoryComponent.h"
#include "RPG_PlayerController.generated.h"

/**
 * 
 */
class URPG_InventoryComponent;
class ARPG_MasterItem;
class URPG_InteractComponent;
UCLASS()
class RPG_SYSTEMS_API ARPG_PlayerController : public APlayerController , public IRPG_PlayerInterface
{
	GENERATED_BODY()
public:

	ARPG_PlayerController();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URPG_InteractComponent* InteractionManager;
	
		virtual void BeginPlay() override;
		// Called every frame
		virtual void Tick(float DeltaTime) override;
		//Bind inputs
		virtual void SetupInputComponent() override;
		
		void Interact();

		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
			void Open_Container(URPG_InventoryComponent* Container);
			virtual void Open_Container_Implementation(URPG_InventoryComponent* Container) override;
			UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
				void Try_PickupItem(ARPG_MasterItem* Item);
			void Try_PickupItem_Implementation(ARPG_MasterItem* Item) override;

			//Try PickupItemServer
			UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
				void Server_OpenContainer(URPG_InventoryComponent* Container);
			bool Server_OpenContainer_Validate(URPG_InventoryComponent* Container);
			void Server_OpenContainer_Implementation(URPG_InventoryComponent* Container);

			UFUNCTION(BlueprintCallable, Client, Reliable)
				void Client_OpenContainer(URPG_InventoryComponent* Container);
			void Client_OpenContainer_Implementation(URPG_InventoryComponent* Container);

			UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
				void OpenHUDContainer(URPG_InventoryComponent* Container);
			virtual void OpenHUDContainer_Implementation(URPG_InventoryComponent* Container);

			//Close Container Hud
			UFUNCTION(BlueprintCallable, Client, Reliable)
				void Client_CloseContainer();
			void Client_CloseContainer_Implementation();
			UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
				void BP_CloseContainer();
			virtual void BP_CloseContainer_Implementation();

};
