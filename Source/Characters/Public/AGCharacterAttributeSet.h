// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AGCharacterAttributeSet.generated.h"

/*
* TODO: Understand these macro definitions. 
*		The ## in  "##PropertyName##"  is a token-pasting operator in C++.
* Macro used in AttributesWidget.cpp
*/
#define NUMERIC_VALUE(AttributeSetName, PropertyName) \
	AttributeSetName->Get##PropertyName##Attribute().GetNumericValue(AttributeSetName)


/**
 *	I'm forced to do pascal case here, or the generated code from the macros get ugly... Sadge.
 */
UCLASS()
class CHARACTERS_API UAGCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
private:

protected:

public:
	UAGCharacterAttributeSet();

	// Called whenever an attribute is about to be modified.
	//	It's meant to enforce things like "Health = Clamp(Health, 0, MaxHealth)" and NOT things like "trigger this extra thing if damage is applied, etc".
	virtual void PreAttributeChange(const FGameplayAttribute& attribute, float& newValue) override;

	// Handle attribute changes through gameplay effects.
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& data) override;
	
	// Whenever we replicate properties, we also have to override this function.
	//   TODO: This has nothing to do with the GAS system???
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* It is recommmended to keep track of both the max and current values.
	 *	This allows you to easily clamp the current value to the max value, and also allows you to easily modify the max value without affecting the current value.
	 *	The ReplicatedUsing specifier designates a callback function that gets executed when the property is updated over the network.
	 *		The callback function can also carry the old value before the modification is done.
	*/

#pragma region  ATTRIBUTE HEALTH
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData Health;
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData MaxHealth;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxHealth);
#pragma endregion
#pragma region  ATTRIBUTE STAMINA
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData Stamina;
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Stamina);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData MaxStamina;
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxStamina);
#pragma endregion
#pragma region  ATTRIBUTE STRENGTH
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData Strength;
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, Strength);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStrength, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData MaxStrength;
	UFUNCTION()
	void OnRep_MaxStrength(const FGameplayAttributeData& OldMaxStrength) const;
	ATTRIBUTE_ACCESSORS_BASIC(ThisClass, MaxStrength);
#pragma endregion
};
