// Copyright dunder.gg. All Rights Reserved.

#include "AGCharacterAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

/*
* These default values should be overriden by a Gameplay Effect blueprint that sets Default Attribute values.
*/
UAGCharacterAttributeSet::UAGCharacterAttributeSet() :
	Health(100.f),
	MaxHealth(100.f),
	Stamina(100.f),
	MaxStamina(100.f),
	Strength(10.f),
	MaxStrength(10.f)
{
	InitHealth(GetMaxHealth());
	InitStamina(GetMaxStamina());
	InitStrength(GetMaxStrength());
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
}

// We trigger in-game reactions to attribute changes here 
void UAGCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& data)
{
	Super::PostGameplayEffectExecute(data);
		// If health reaches 0, then we die.
	if (data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Set our new health value, but make sure it is between 0 and max health.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
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
