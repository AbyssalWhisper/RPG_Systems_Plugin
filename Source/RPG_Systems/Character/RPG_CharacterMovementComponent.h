// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPG_CharacterMovementComponent.generated.h"

class ARPG_BaseCharacter;
/**
 * 
 */
UCLASS()
class RPG_SYSTEMS_API URPG_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
		UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		ARPG_BaseCharacter* OwnerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector CustomGravity;
	virtual void BeginPlay() override;
	virtual float GetMaxSpeed() const override;
	
};
