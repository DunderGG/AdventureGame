// Copyright dunder.gg. All Rights Reserved.

#include "AGCharacterAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

/*
* These default values should be overriden by a Gameplay Effect blueprint that sets Default Attribute values.
*/
UAGCharacterAttributeSet::UAGCharacterAttributeSet() :
	Health(1.f),
	MaxHealth(100.f),
	Stamina(1.f),
	MaxStamina(100.f),
	Strength(1.f),
	MaxStrength(10.f),
	CharacterLevel(1)
{
	InitHealth(GetMaxHealth());
	InitStamina(GetMaxStamina());
	InitStrength(GetMaxStrength());
	InitCharacterLevel(1);
}

// This is used to "clean up" values for attribute changes.
// It is not the right place to trigger in-game reactions to attribute changes, that is done in PostGameplayEffectExecute.
void UAGCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& attribute, float& newValue)
{
	Super::PreAttributeChange(attribute, newValue);

	if (attribute == GetHealthAttribute())
	{
		// Make sure new current health is between 0 and max health.
		newValue = FMath::Clamp<float>(newValue, 0.0f, GetMaxHealth());
	}
	else if (attribute == GetMaxHealthAttribute())
	{
		// Max health is often used as a divisor for calculating health percentages (in the UI for example), so we want to make sure it is never 0 or negative.
		newValue = FMath::Max<float>(1.0f, newValue);
	}
	else if (attribute == GetStaminaAttribute())
	{
		newValue = FMath::Clamp<float>(newValue, 0.0f, GetMaxStamina());
	}
	else if (attribute == GetMaxStaminaAttribute())
	{
		newValue = FMath::Max<float>(1.0f, newValue);
	}
	else if (attribute == GetStrengthAttribute())
	{
		newValue = FMath::Clamp<float>(newValue, 0.0f, GetMaxStrength());
	}
	else if (attribute == GetMaxStrengthAttribute())
	{
		newValue = FMath::Max<float>(1.0f, newValue);
	}

	// TODO: It has been suggested to add a tag when attributes are "full" or "empty",
	//         so it doesn't pointlessly try to add more when already full.

}

// We trigger in-game reactions to attribute changes here 
// Apparently, sometimes periodic gameplay effects can skip the PreAttributeChange() function,
//	so we have to make sure to clamp the values here as well.
//     TODO: Is there even any point in clamping in PreAttributeChange() if we have to do it here anyway???
void UAGCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& data)
{
	Super::PostGameplayEffectExecute(data);

	// TODO: If health reaches 0, then we die.
	if (data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Set our new health value, but make sure it is between 0 and max health.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	/*
	* The data reference also gives us access to the target effect spec.
	* Through the data reference, 
		we can access the target actor (through data.Target), 
		the instigator of the effect, 
			its ASC, location, etc...
	UAbilitySystemComponent* ASC = data.EffectSpec.GetContext().GetInstigatorAbilitySystemComponent();
	AActor* ASCOwner = ASC->AbilityActorInfo->OwnerActor.Get();
	ASCOwner->GetActorLocation();
	*/
}

void UAGCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Done in order to make the replication work. This comes from the UnrealNetwork header.
	//	The property will only be replicated to connections where the None condition is met (i.e. always) and 
	//		will trigger a REPNOTIFY when the second condition is met (i.e. always).
	DOREPLIFETIME_CONDITION_NOTIFY(UAGCharacterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGCharacterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGCharacterAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGCharacterAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGCharacterAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGCharacterAttributeSet, MaxStrength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGCharacterAttributeSet, CharacterLevel, COND_None, REPNOTIFY_Always);
}

void UAGCharacterAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// Here we inform the GAS system about the attribute change, so that it can do any necessary background stuff.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGCharacterAttributeSet, Health, OldHealth);
}
void UAGCharacterAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGCharacterAttributeSet, MaxHealth, OldMaxHealth);
}

void UAGCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGCharacterAttributeSet, Stamina, OldStamina);
}

void UAGCharacterAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGCharacterAttributeSet, MaxStamina, OldMaxStamina);
}

void UAGCharacterAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGCharacterAttributeSet, Strength, OldStrength);
}

void UAGCharacterAttributeSet::OnRep_MaxStrength(const FGameplayAttributeData& OldMaxStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGCharacterAttributeSet, MaxStrength, OldMaxStrength);
}

void UAGCharacterAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGCharacterAttributeSet, CharacterLevel, OldCharacterLevel);
}

