// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Systems/GameplayAbility/RPG_BaseAttributeSet.h"
#include "GameFramework/Actor.h"
#include "RPG_AbilitySystemActor.generated.h"

class UAbilitySystemComponent;


UCLASS()
class RPG_SYSTEMS_API ARPG_AbilitySystemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPG_AbilitySystemActor();
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UAbilitySystemComponent* AbilitySystemComp;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		URPG_BaseAttributeSet* AttributesSet;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
