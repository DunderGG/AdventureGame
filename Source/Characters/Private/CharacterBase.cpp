// Copyright dunder.gg. All Rights Reserved.

#include "CharacterBase.h"
#include "PlayerAbilitySystemComponent.h"
#include "PlayerAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InventoryComponent.h"
// TODO: This needs refactoring so we don't include loads of singular gameplay effects.
#include "BaseStaminaRecovery.h"
#include "SprintStaminaExpense.h"
#include "BaseHealthRecovery.h"
#include "SetDefaultAttributes.h"
#include "GAJump.h"
///////////////////
#include "AdventureGameplayTags.h"
#include "Logger.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	Logger::addMessage(TEXT("ACharacterBase::ACharacterBase(): Constructing new ACharacterBase"), SEVERITY::Debug, true, true, false);

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("inventory"));
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (getStamina() <= 0 && isSprinting)
	{
		Logger::addMessage(TEXT("ACharacterBase::Tick(): Stamina depleted, stopping sprint"), SEVERITY::Info);
		setSprinting(false);
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = getWalkSpeed();
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{ return abilitySystemComponent.Get(); }

UPlayerAttributeSet* ACharacterBase::GetAttributeSet() const
{ return attributeSet.Get(); }


bool ACharacterBase::canCharacterJump() const
{
	// CanJump() is an UE ACharacter function, may want to override it
	return ACharacter::CanJump();
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

void ACharacterBase::applyStaminaDepletion()
{
	// When sprinting, we want to stop stamina recovery and start stamina depletion.
	if (abilitySystemComponent)
	{
		// Remove base stamina recovery effect
		abilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(UBaseStaminaRecovery::StaticClass(), nullptr, -1);

		// Apply sprint stamina depletion effect
		FGameplayEffectContextHandle effectContext = abilitySystemComponent->MakeEffectContext();
		effectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle depletionSpec = abilitySystemComponent->MakeOutgoingSpec(USprintStaminaExpense::StaticClass(), 1, effectContext);
		if (depletionSpec.IsValid())
		{
			abilitySystemComponent->ApplyGameplayEffectSpecToSelf(*depletionSpec.Data.Get());
		}
		else
		{
			Logger::addMessage(TEXT("APlayerCharacter::sprintOn(): Failed to create SprintStaminaExpense spec"), SEVERITY::Error);
		}
	}
}
//TODO: We should probably refactor these two apply functions into separate apply/remove-functions.
void ACharacterBase::applyStaminaRecovery()
{
	// When stopping sprinting, we want to stop stamina depletion and start stamina recovery again.
	if (abilitySystemComponent)
	{
		// 1. Remove sprint stamina depletion effect
		abilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(USprintStaminaExpense::StaticClass(), nullptr, -1);

		// 2. Check if the base recovery effect gameplay tag is already active before adding another recovery effect
		if (abilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.State.IsBaseRecoveringStamina"))))
		{
			Logger::addMessage(TEXT("APlayerCharacter::sprintOff(): Base stamina recovery effect already active, not applying another"), SEVERITY::Info);
			return;
		}

		// 3. Re-apply base stamina recovery effect
		FGameplayEffectContextHandle effectContext = abilitySystemComponent->MakeEffectContext();
		effectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle recoverySpec = abilitySystemComponent->MakeOutgoingSpec(UBaseStaminaRecovery::StaticClass(), 1, effectContext);
		if (recoverySpec.IsValid())
		{
			abilitySystemComponent->ApplyGameplayEffectSpecToSelf(*recoverySpec.Data.Get());
		}
		else
		{
			Logger::addMessage(TEXT("APlayerCharacter::sprintOff(): Failed to create BaseStaminaRecovery spec"), SEVERITY::Error);
		}
	}
}

void ACharacterBase::setSprinting(const bool newIsSprinting)
{
	if (newIsSprinting)
	{
		// Only allow sprinting if we have enough stamina
		if (getStamina() < 10)
		{
			Logger::addMessage(TEXT("ACharacterBase::setSprinting(): Not enough stamina to start sprinting"), SEVERITY::Info);
			return;
		}
		applyStaminaDepletion();
		GetCharacterMovement()->MaxWalkSpeed = getSprintSpeed();
		isSprinting = true;
		return;
	}
	else
	{
		applyStaminaRecovery();
	}
	
	// Take care of scenario where player is holding shift to sprint,
	//	then presses ctrl to sneak while still holding shift,
	//	and then lets go off shift, calling setSprinting(false).
	//		We do not want MaxWalkSpeed to be set to our walk speed
	isSprinting = false;
	if (isSneaking)
	{
		// If we stop sprinting but still hold sneak button, resume sneaking.
		GetCharacterMovement()->MaxWalkSpeed = getSneakSpeed();
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = getWalkSpeed();
	return;		// Not needed of course, but it makes it clear that nothing was accidently removed from this function
}

void ACharacterBase::setSneaking(const bool newIsSneaking)
{
	if (newIsSneaking)
	{
		isSneaking = true;
		GetCharacterMovement()->MaxWalkSpeed = getSneakSpeed();
		return;
	}

	// Same as the special scenario described in setSprinting()
	//		We do not want MaxWalkSpeed to be set to our walk speed
	//	TODO: For the future, this could be a sprint and slide functionality to add
	isSneaking = false;
	if (isSprinting)
	{
		// If we stop sneaking but still hold sprint button, resume sprinting.
		GetCharacterMovement()->MaxWalkSpeed = getSprintSpeed();
		return;
	}
	//TODO: Probably need another check for if we are jumping. Currently we are slowing down mid-air if we "sneak".
	GetCharacterMovement()->MaxWalkSpeed = getWalkSpeed();
	return;
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UInventoryComponent* ACharacterBase::getInventory() const
{
	return inventoryComponent;
}

#pragma region Attribute Set Getters
int32 ACharacterBase::getCharacterLevel() const
{
	if (attributeSet)
	{
		return static_cast<int32>(attributeSet->GetCharacterLevel());
	}
	else
	{
		Logger::addMessage(TEXT("ACharacterBase::getCharacterLevel(): AttributeSet not yet initialized"), SEVERITY::Error);
	}

	return 0;
}

float ACharacterBase::getHealth() const
{
	if (attributeSet)
	{
		return attributeSet->GetHealth();
	}
	else
	{
		Logger::addMessage(TEXT("ACharacterBase::getHealth(): AttributeSet not yet initialized"), SEVERITY::Error);
	}

	return 0.0f;
}

float ACharacterBase::getMaxHealth() const
{
	if (attributeSet)
	{
		return attributeSet->GetMaxHealth();
	}
	else
	{
		Logger::addMessage(TEXT("ACharacterBase::getMaxHealth(): AttributeSet not yet initialized"), SEVERITY::Error);
	}

	return 0.0f;
}

float ACharacterBase::getStamina() const
{
	if (attributeSet)
	{
		return attributeSet->GetStamina();
	}
	else
	{
		Logger::addMessage(TEXT("ACharacterBase::getStamina(): AttributeSet not yet initialized"), SEVERITY::Error);
	}

	return 0.0f;
}

float ACharacterBase::getMaxStamina() const
{
	if (attributeSet)
	{
		return attributeSet->GetMaxStamina();
	}
	else
	{
		Logger::addMessage(TEXT("ACharacterBase::getMaxStamina(): AttributeSet not yet initialized"), SEVERITY::Error);
	}

	return 0.0f;
}

float ACharacterBase::getStrength() const
{
	if (attributeSet)
	{
		return attributeSet->GetStrength();
	}
	else
	{
		Logger::addMessage(TEXT("ACharacterBase::getStrength(): AttributeSet not yet initialized"), SEVERITY::Error);
	}

	return 0.0f;
}

float ACharacterBase::getMaxStrength() const
{
	if (attributeSet)
	{
		return attributeSet->GetMaxStrength();
	}
	else
	{
		Logger::addMessage(TEXT("ACharacterBase::getMaxStrength(): AttributeSet not yet initialized"), SEVERITY::Error);
	}

	return 0.0f;
}
#pragma endregion

/*
* Apply the list of starting effects to the character once at startup.
*/
void ACharacterBase::initStartupEffects()
{
	startupEffects.Add(UBaseStaminaRecovery::StaticClass());
	startupEffects.Add(UBaseHealthRecovery::StaticClass());
}
void ACharacterBase::applyStartupEffects()
{
	if (abilitySystemComponent)
	{
		if (abilitySystemComponent->areStartupEffectsApplied)
		{
			Logger::addMessage(TEXT("ACharacterBase::giveStartupEffects(): Startup effects have already been applied"), SEVERITY::Warning);
			return;
		}
		if (GetLocalRole() != ROLE_Authority)
		{
			Logger::addMessage(TEXT("ACharacterBase::giveStartupEffects(): Don't have ROLE_Authority"), SEVERITY::Error);
			return;
		}

		FGameplayEffectContextHandle effectContext = abilitySystemComponent->MakeEffectContext();
		effectContext.AddSourceObject(this);

		int index = 0;
		// The startupEffects array is populated in the editor.
		for (TSubclassOf<UGameplayEffect> gameplayEffect : startupEffects)
		{
			FGameplayEffectSpecHandle newEffect = abilitySystemComponent->MakeOutgoingSpec(gameplayEffect, getCharacterLevel(), effectContext);
			if (newEffect.IsValid())
			{
				FActiveGameplayEffectHandle activeGE = abilitySystemComponent->ApplyGameplayEffectSpecToSelf(*newEffect.Data.Get());
			}
			else
			{
				Logger::addMessage(FString::Printf(TEXT("ACharacterBase::giveStartupEffects(): New effect at index %d not valid"), index), SEVERITY::Error);
			}
			++index;
		}
		Logger::addMessage(FString::Printf(TEXT("ACharacterBase::giveStartupEffects(): %d Startup effects applied"), startupEffects.Num()), SEVERITY::Debug);
		abilitySystemComponent->areStartupEffectsApplied = true;
	}
	else
	{
		Logger::addMessage(TEXT("ACharacterBase::giveStartupEffects(): AbilitySystemComponent not yet initialized"), SEVERITY::Error);
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
void ACharacterBase::applyDefaultAttributes()
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
				Logger::addMessage(TEXT("ACharacterBase::giveDefaultAttributes(): specHandle not valid"), SEVERITY::Error);
			}
		}
		else
		{
			Logger::addMessage(TEXT("ACharacterBase::giveDefaultAttributes(): defaultAttributesEffect not initialized"), SEVERITY::Error);
		}
	}
	else
	{
		Logger::addMessage(TEXT("ACharacterBase::giveDefaultAttributes(): AbilitySystemComponent not yet initialized"), SEVERITY::Error);
	}
}

/*
* We have not created any abilities yet, 
* but this is where we would give the character their default abilities, like "Kick".
* 
* Since abilities have animations and stuff, 
* it is probably easier to create them in the editor as Gameplay Ability blueprints, 
* and then add those blueprints to the defaultAbilities array in the editor.
*/
void ACharacterBase::initDefaultAbilities()
{
	defaultAbilities.Add(UGAJump::StaticClass());
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
			int index = 0;
			// Loop through all the default abilities
			for (TSubclassOf<UGameplayAbility>& startupAbility : defaultAbilities)
			{
				// Create a gameplay ability at level 1 and use the ASC to give that to the actor.
				const FGameplayAbilitySpec abilitySpec(startupAbility, 1);
				if (abilitySpec.Ability)
				{
					abilitySystemComponent->GiveAbility(abilitySpec);
				}
				else
				{
					Logger::addMessage(FString::Printf(TEXT("ACharacterBase::giveDefaultAbilities(): Ability at index %d not valid"), index), SEVERITY::Error);
				}
				++index;
			}
			// TODO: Remember to set this to false in removeCharacterAbilities().
			abilitySystemComponent->areStartupEffectsApplied = true;
			Logger::addMessage(FString::Printf(TEXT("ACharacterBase::giveDefaultAbilities(): %d Default abilities given"), defaultAbilities.Num()), SEVERITY::Debug);
		}
		else
		{
			Logger::addMessage(TEXT("ACharacterBase::giveDefaultAbilities(): Don't have network authority"), SEVERITY::Error);
		}
	}
	else
	{
		Logger::addMessage(TEXT("ACharacterBase::giveDefaultAbilities(): AbilitySystemComponent not yet initialized"), SEVERITY::Error);
	}
}
