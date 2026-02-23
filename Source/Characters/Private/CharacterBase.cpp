// Copyright dunder.gg. All Rights Reserved.

#include "CharacterBase.h"
#include "AGAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AGCharacterAttributeSet.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = getWalkSpeed();
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{ return abilitySystemComponent.Get(); }

UAGCharacterAttributeSet* ACharacterBase::GetAttributeSet() const
{ return attributeSet.Get(); }


bool ACharacterBase::canCharacterJump() const
{
	// CanJump() is an UE ACharacter function, may want to override it
	return CanJump();
}

void ACharacterBase::hasJumped()
{
	ACharacter::Jump();
}

float ACharacterBase::getSneakSpeed() const
{
	return sneakSpeed;
}

float ACharacterBase::getWalkSpeed() const
{
	return walkSpeed;
}

float ACharacterBase::getSprintSpeed() const
{
	return sprintSpeed;
}

void ACharacterBase::setSprinting(const bool newIsSprinting)
{
	if (newIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = getSprintSpeed();
		isSprinting = true;
		isSneaking = false;
		return;
	}
	
	// Take care of scenario where player is holding shift to sprint,
	//	then presses ctrl to sneak while still holding shift,
	//	and then lets go off shift, calling setSprinting(false).
	//		We do not want MaxWalkSpeed to be set to our walk speed
	if (isSneaking)
	{
		return;
	}

	isSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = getWalkSpeed();
	return;		// Not needed of course, but it makes it clear that nothing was accidently removed from this function
}

void ACharacterBase::setSneaking(const bool newIsSneaking)
{
	if (newIsSneaking)
	{
		isSprinting = false;
		isSneaking = true;
		GetCharacterMovement()->MaxWalkSpeed = getSneakSpeed();
		return;
	}

	// Same as the special scenario described in setSprinting()
	//		We do not want MaxWalkSpeed to be set to our walk speed
	//	TODO: For the future, this could be a sprint and slide functionality to add
	if (isSprinting)
	{
		return;
	}
	//TODO: Probably need another check for if we are jumping. Currently we are slowing down mid-air if we "sneak".
	isSneaking = false;
	GetCharacterMovement()->MaxWalkSpeed = getWalkSpeed();
	return;
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

#pragma region Attribute Set Getters
int32 ACharacterBase::getCharacterLevel() const
{
	if (attributeSet)
	{
		return static_cast<int32>(attributeSet->GetCharacterLevel());
	}

	return 0;
}

float ACharacterBase::getHealth() const
{
	if (attributeSet)
	{
		return static_cast<int32>(attributeSet->GetHealth());
	}

	return 0.0f;
}

float ACharacterBase::getMaxHealth() const
{
	if (attributeSet)
	{
		return static_cast<int32>(attributeSet->GetMaxHealth());
	}

	return 0.0f;
}

float ACharacterBase::getStamina() const
{
	if (attributeSet)
	{
		return static_cast<int32>(attributeSet->GetStamina());
	}

	return 0.0f;
}

float ACharacterBase::getMaxStamina() const
{
	if (attributeSet)
	{
		return static_cast<int32>(attributeSet->GetMaxStamina());
	}

	return 0.0f;
}

float ACharacterBase::getStrength() const
{
	if (attributeSet)
	{
		return static_cast<int32>(attributeSet->GetStrength());
	}

	return 0.0f;
}

float ACharacterBase::getMaxStrength() const
{
	if (attributeSet)
	{
		return static_cast<int32>(attributeSet->GetMaxStrength());
	}

	return 0.0f;
}
#pragma endregion

/*
* Apply the list of starting effects to the character once at startup.
*/
void ACharacterBase::giveStartupEffects()
{
	if (abilitySystemComponent)
	{
		if (abilitySystemComponent->areStartupEffectsApplied)
		{
			UE_LOG(LogTemp, Warning, TEXT("ACharacterBase::giveStartupEffects(): Startup effects have already been applied"));
			return;
		}
		if (GetLocalRole() != ROLE_Authority)
		{
			UE_LOG(LogTemp, Error, TEXT("ACharacterBase::giveStartupEffects(): Don't have ROLE_Authority"));
			return;
		}

		FGameplayEffectContextHandle effectContext = abilitySystemComponent->MakeEffectContext();
		effectContext.AddSourceObject(this);

		for (TSubclassOf<UGameplayEffect> gameplayEffect : startupEffects)
		{
			FGameplayEffectSpecHandle newEffect = abilitySystemComponent->MakeOutgoingSpec(gameplayEffect, getCharacterLevel(), effectContext);
			if (newEffect.IsValid())
			{
				FActiveGameplayEffectHandle activeGE = abilitySystemComponent->ApplyGameplayEffectSpecToSelf(*newEffect.Data.Get());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ACharacterBase::giveStartupEffects(): New effect not valid"));
			}
		}
		abilitySystemComponent->areStartupEffectsApplied = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACharacterBase::giveStartupEffects(): AbilitySystemComponent not yet initialized"));
	}
}

void ACharacterBase::initStartupEffects()
{
	// TODO: This is ugly... Maybe we shouldn't have separate classes and just combine them.
	//	 Would also remove the for-loop in giveStartupEffects().
	startupEffects.Add(UBaseStaminaRecovery::StaticClass());
	startupEffects.Add(UBaseHealthRecovery::StaticClass());
}

/*
* Use a gameplay effect to initialise default attribute values.
*/
void ACharacterBase::giveDefaultAttributes()
{
	// Check that ASC and default values exist
	if (abilitySystemComponent)
	{
		if (defaultAttributesEffect)
		{
			// This is a wrapper that holds an FGameplayEffectContext,
			//	which holds the instigator of the gameplay effect, and related data like positions and targets.
			FGameplayEffectContextHandle effectContext = abilitySystemComponent->MakeEffectContext();
			// The source here is this character.
			effectContext.AddSourceObject(this);

			// We also need an outgoing effect specification.
			//	This holds a data pointer that can be used to reference the effect specification.
			const FGameplayEffectSpecHandle specHandle = abilitySystemComponent->MakeOutgoingSpec(defaultAttributesEffect, 1, effectContext);

			// Finally, if valid, apply the gameplay effect to the character
			if (specHandle.IsValid())
			{
				abilitySystemComponent->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ACharacterBase::giveDefaultAttributes(): specHandle not valid"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ACharacterBase::giveDefaultAttributes(): defaultAttributesEffect not initialized"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACharacterBase::giveDefaultAttributes(): AbilitySystemComponent not yet initialized"));
	}
}

/*
* Defines a gameplay effect that sets all the default attribute values.
* Recommended by Epic apparently.
*    Very easy to do in the editor, not as easy to do in code, but it is what it is...
*/
void ACharacterBase::initDefaultAttributes()
{
	defaultAttributesEffect = USetDefaultAttributes::StaticClass();
}

void ACharacterBase::giveDefaultAbilities()
{
	// Make sure the ASC has been initialised.
	if (abilitySystemComponent)
	{
		// Before giving abilities, make sure the local role of the actor is to have network authority???
		// TODO: No idea what that means, look it up.
		if (HasAuthority())
		{
			// Loop through all the default abilities
			for (TSubclassOf<UGameplayAbility>& startupAbility : defaultAbilities)
			{
				// Create a gameplay ability at level 1 and use the ASC to give that to the actor.
				const FGameplayAbilitySpec abilitySpec(startupAbility, 1);
				abilitySystemComponent->GiveAbility(abilitySpec);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ACharacterBase::giveDefaultAbilities(): Don't have network authority"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACharacterBase::giveDefaultAbilities(): AbilitySystemComponent not yet initialized"));
	}
}


