// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/GameplayAbility/RPG_AbilitySystemActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
ARPG_AbilitySystemActor::ARPG_AbilitySystemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);
	AttributesSet = CreateDefaultSubobject<URPG_BaseAttributeSet>("Attributes");
}

// Called when the game starts or when spawned
void ARPG_AbilitySystemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPG_AbilitySystemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

