// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_CheckAttribute.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

bool URPG_CheckAttribute::CanUse_Implementation(AActor* Owner)
{
	// Check if Owner is valid
    if (!Owner)
        return false;

    // Get the Ability System Component of the Owner
    UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Owner);
    if (!AbilitySystemComponent)
        return false;

    // Check if the Target Attribute is valid
    if (!TargetAttribute.IsValid())
    {
        // Log a warning if the Target Attribute is invalid and return false
        UE_LOG(LogTemp, Warning, TEXT("Invalid target attribute."));
        return false;
    }

    // Get the numeric value of the Target Attribute
    float TargetValue = AbilitySystemComponent->GetNumericAttribute(TargetAttribute);

    // Value to be compared
    float CompareValue = 0.0f;

    // If bUseAttributeToCompare is true, update CompareValue with the value of AttributeToCompare
    if (bUseAttributeToCompare)
    {
        // Check if the AttributeToCompare is valid
        if (!AttributeToCompare.IsValid())
        {
            // Log a warning if the AttributeToCompare is invalid and return false
            UE_LOG(LogTemp, Warning, TEXT("Invalid comparison attribute."));
            return false;
        }

        // Get the numeric value of the AttributeToCompare
        CompareValue = AbilitySystemComponent->GetNumericAttribute(AttributeToCompare);

        // If necessary, adjust the comparison value for percentage
        if (bPercentageCompare)
        {
            CompareValue *= (PercentageValue * 0.01f); // Multiply by PercentageValue to obtain the percentage
        }
    }
    else // If not using the AttributeToCompare, use Value to set CompareValue
    {
        CompareValue = Value;
    }

    // Perform comparison based on the ComparisonMethod
    switch (ComparisonMethod)
    {
        case EComparisonMethod::Equal_To:
            return TargetValue == CompareValue;
        case EComparisonMethod::Not_Equal_To:
            return TargetValue != CompareValue;
        case EComparisonMethod::Greater_Than_Or_Equal_To:
            return TargetValue >= CompareValue;
        case EComparisonMethod::Less_Than_Or_Equal_To:
            return TargetValue <= CompareValue;
        case EComparisonMethod::Greater_Than:
            return TargetValue > CompareValue;
        case EComparisonMethod::Less_Than:
            return TargetValue < CompareValue;
        default:
            return false;
    }
}

FText URPG_CheckAttribute::GetCantUseMensage_Implementation()
{
    // Return FText with the specified message for localization
    return NSLOCTEXT("URPG_CheckAttribute", "CannotUseItem", "You cannot use this item.");

}
