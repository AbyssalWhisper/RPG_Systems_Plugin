// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "Components/ActorComponent.h"
#include "RPG_AttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeChanged,float, NewValue,float, OldValue);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_SYSTEMS_API URPG_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URPG_AttributeComponent();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayAttribute Attribute;
protected:
	
	void OnAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	UFUNCTION(BlueprintImplementableEvent, Category = "Attribute")
	void BP_OnAttributeChanged(float NewValue,float OldValue);
	UPROPERTY(BlueprintAssignable, Category = "Attribute")
	FAttributeChanged OnAttributeChangeDelegate;
	// Called when the game starts
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
