


#include "RPG_Systems/Footprint/RPG_FootprintActor.h"

#include "Components/DecalComponent.h"

// Sets default values
ARPG_FootprintActor::ARPG_FootprintActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	SetRootComponent(SceneComp);
	SceneComp->Mobility = EComponentMobility::Movable;
	DecalComponent = CreateDefaultSubobject<UDecalComponent>("Decal");
	DecalComponent->SetupAttachment(SceneComp);
	DecalComponent->SetRelativeRotation(FRotator(-90,0,90));
	DecalComponent->DecalSize = FVector(5,10,10);
}

// Called when the game starts or when spawned
void ARPG_FootprintActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPG_FootprintActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

