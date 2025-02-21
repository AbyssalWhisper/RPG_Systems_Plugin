// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "CustomCapsuleComponent.generated.h"

UCLASS(ClassGroup=(Collision), meta=(BlueprintSpawnableComponent))
class RPG_SYSTEMS_API UCustomCapsuleComponent : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UCustomCapsuleComponent();

	/** Define um deslocamento para a colisão da cápsula */
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void SetCapsuleCenter(FVector NewCenter);

protected:
	/** Armazena o deslocamento do centro da cápsula */
	FVector CapsuleCenterOffset;

	virtual void UpdateBodySetup() override;

	///~ Begin UPrimitiveComponent Interface.
	FPrimitiveSceneProxy* CreateSceneProxy() override;
	///~ End UPrimitiveComponent Interface.
};
