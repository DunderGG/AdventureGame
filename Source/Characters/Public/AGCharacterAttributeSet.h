// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AGCharacterAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeDataChanged, float, OldValue, float, NewValue);
/**
 *	I'm forced to do pascal case here, or the generated code from the macros get ugly... Sadge.
 */
UCLASS()
class CHARACTERS_API UAGCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAGCharacterAttributeSet();

	// These macros exist so we don't have to define so many accessors ourselves for all our attributes.
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Health);

	// Called whenever an attribute is about to be modified.
	//	It's meant to enforce things like "Health = Clamp(Health, 0, MaxHealth)" and NOT things like "trigger this extra thing if damage is applied, etc".
	virtual void PreAttributeChange(const FGameplayAttribute& attribute, float& newValue) override;

protected:
	// It is recommmended to keep track of both the max and current values. 
	// This allows you to easily clamp the current value to the max value, and also allows you to easily modify the max value without affecting the current value.
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Health;
};
