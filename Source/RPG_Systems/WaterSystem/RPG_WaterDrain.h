

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPG_WaterDrain.generated.h"

class UBoxComponent;
class UPostProcessComponent;

USTRUCT(BlueprintType)
struct  FSTR_MoveActorsWaterSystem
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Actor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector OffSetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector OffSetScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UpdateScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UpdateRotation;
	FSTR_MoveActorsWaterSystem() {
		Actor = nullptr;
		OffSetLocation = FVector::Zero();
		OffSetScale = FVector(1);
		UpdateScale = true;
		UpdateRotation = true;
	}
};

UCLASS()
class RPG_SYSTEMS_API ARPG_WaterDrain : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARPG_WaterDrain();
 
	UPROPERTY()
		APhysicsVolume* PhysicsVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterMovement)
	float TerminalVelocity = 4000.0;

	/** Determines which PhysicsVolume takes precedence if they overlap (higher number = higher priority). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterMovement)
	int32 Priority;

	/** This property controls the amount of friction applied by the volume as pawns using CharacterMovement move through it. The higher this value, the harder it will feel to move through */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CharacterMovement)
	float FluidFriction  =0.3f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FSTR_MoveActorsWaterSystem> Actors;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* BoxComponent;
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	UPostProcessComponent* PostProcess;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		float MaxWater = 100;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_CurrentWater)
		float CurrentWater = 100;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, ReplicatedUsing = OnRep_CurrentWater)
		float WaterHeight;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		bool WaterValueCanChange = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool UpdateWaterByTime = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* WaterMesh;
	UPROPERTY()
		FTimerHandle TimeHandle; 
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<ECollisionChannel> WaterCollisionChannel;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public: 
	//UFUNCTION(BlueprintCallable)
		//void UpdateWaterStartLocation();
	
	UFUNCTION(BlueprintCallable)
		virtual	void UpdateWater();
	UFUNCTION(BlueprintPure)
		FGuid GetGuid();
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Meta = (ExposeOnSpawn = true), NonPIEDuplicateTransient, NonTransactional)
		FGuid ActorWorldGuid;
	UFUNCTION(BlueprintCallable)
		virtual void InitGuid();
	UFUNCTION(BlueprintPure)
		bool GetIsMapSpawnActor();
	virtual void OnConstruction(const FTransform& Transform) override;
	UFUNCTION(BlueprintCallable)
		void AddWater(float Water);
	UFUNCTION(BlueprintCallable)
		void SetWaterPercent(float Percent);
	UFUNCTION(BlueprintCallable)
		void WaterUpdateTick();
	UFUNCTION(BlueprintNativeEvent)
		float GetWaterUpdateValue();
		float GetWaterUpdateValue_Implementation();
	UFUNCTION(BlueprintCallable)
		virtual void OnRep_CurrentWater();
	UFUNCTION(BlueprintNativeEvent)
		void OnWaterHeightChange(float Value);
	virtual void OnWaterHeightChange_Implementation(float Value);

	UFUNCTION(BlueprintNativeEvent)
		void OnWaterValueChange(float Value);
	virtual void OnWaterValueChange_Implementation(float Value);
};
