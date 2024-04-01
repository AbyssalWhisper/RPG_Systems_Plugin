

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPG_FootprintActor.generated.h"

UCLASS()
class RPG_SYSTEMS_API ARPG_FootprintActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPG_FootprintActor();
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	AActor* OwnerActor;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	UDecalComponent* DecalComponent;
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	USceneComponent* SceneComp;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
