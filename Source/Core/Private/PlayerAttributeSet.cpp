// Copyright (C) 2026 dunder.gg [GNU GPLv3]


#include "PlayerAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Logger.h"

UPlayerAttributeSet::UPlayerAttributeSet()
{
	Logger::addMessage(TEXT("UPlayerAttributeSet::UPlayerAttributeSet(): Constructing new UPlayerAttributeSet"), SEVERITY::Debug, true, true, false);
}

// This is used to "clean up" values for attribute changes.
// It is not the right place to trigger in-game reactions to attribute changes, that is done in PostGameplayEffectExecute.
void UPlayerAttributeSet::PreAttributeChange(const FGameplayAttribute& attribute, float& newValue)
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
	else if (attribute == GetCharacterLevelAttribute())
	{
		newValue = FMath::Max<float>(1.0f, newValue);
	}
	else if (attribute == GetMoveSpeedAttribute())
	{
		newValue = FMath::Max<float>(0.0f, newValue);
	}
	else if (attribute == GetSprintSpeedAttribute())
	{
		newValue = FMath::Max<float>(0.0f, newValue);
	}
	else if (attribute == GetSneakSpeedAttribute())
	{
		newValue = FMath::Max<float>(0.0f, newValue);
	}
	else if (attribute == GetNoiseAttribute())
	{
		newValue = FMath::Max<float>(0.0f, newValue);
	}

	// TODO: It has been suggested to add a tag when attributes are "full" or "empty",
	//         so it doesn't pointlessly try to add more when already full.

}

// We trigger in-game reactions to attribute changes here 
// Apparently, sometimes periodic gameplay effects can skip the PreAttributeChange() function,
//	so we have to make sure to clamp the values here as well.
//     TODO: Is there even any point in clamping in PreAttributeChange() if we have to do it here anyway???
void UPlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& data)
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
	else if (data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		SetMoveSpeed(FMath::Max<float>(0.0f, GetMoveSpeed()));
	}
	else if (data.EvaluatedData.Attribute == GetSprintSpeedAttribute())
	{
		SetSprintSpeed(FMath::Max<float>(0.0f, GetSprintSpeed()));
	}
	else if (data.EvaluatedData.Attribute == GetSneakSpeedAttribute())
	{
		SetSneakSpeed(FMath::Max<float>(0.0f, GetSneakSpeed()));
	}
	else if (data.EvaluatedData.Attribute == GetNoiseAttribute())
	{
		SetNoise(FMath::Max<float>(0.0f, GetNoise()));
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

void UPlayerAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Done in order to make the replication work. This comes from the UnrealNetwork header.
	//	The property will only be replicated to connections where the None condition is met (i.e. always) and 
	//		will trigger a REPNOTIFY when the second condition is met (i.e. always).
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MaxStrength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, SprintSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, SneakSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPlayerAttributeSet, Noise, COND_None, REPNOTIFY_Always);
}

void UPlayerAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// Here we inform the GAS system about the attribute change, so that it can do any necessary background stuff.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Health, OldHealth);
}
void UPlayerAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxHealth, OldMaxHealth);
}

void UPlayerAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Stamina, OldStamina);
}

void UPlayerAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxStamina, OldMaxStamina);
}

void UPlayerAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Strength, OldStrength);
}

void UPlayerAttributeSet::OnRep_MaxStrength(const FGameplayAttributeData& OldMaxStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MaxStrength, OldMaxStrength);
}

void UPlayerAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, CharacterLevel, OldCharacterLevel);
}

void UPlayerAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, MoveSpeed, OldMoveSpeed);
}

void UPlayerAttributeSet::OnRep_SprintSpeed(const FGameplayAttributeData& OldSprintSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, SprintSpeed, OldSprintSpeed);
}

void UPlayerAttributeSet::OnRep_SneakSpeed(const FGameplayAttributeData& OldSneakSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, SneakSpeed, OldSneakSpeed);
}

void UPlayerAttributeSet::OnRep_Noise(const FGameplayAttributeData& OldNoise) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPlayerAttributeSet, Noise, OldNoise);
}
