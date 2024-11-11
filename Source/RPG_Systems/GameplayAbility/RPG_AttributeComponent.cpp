// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Systems/GameplayAbility/RPG_AttributeComponent.h"

#include "AbilitySystemComponent.h"

// Sets default values for this component's properties
URPG_AttributeComponent::URPG_AttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


void URPG_AttributeComponent::OnAttributeChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	BP_OnAttributeChanged(OnAttributeChangeData.NewValue,OnAttributeChangeData.OldValue);
	OnAttributeChangeDelegate.Broadcast(OnAttributeChangeData.NewValue,OnAttributeChangeData.OldValue);
}

// Called when the game starts
void URPG_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner())
	{
		UAbilitySystemComponent* asc = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
		asc->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this,&URPG_AttributeComponent::OnAttributeChanged);
	}
	// ...
	
}


// Called every frame
void URPG_AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

