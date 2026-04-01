// Copyright dunder.gg. All Rights Reserved.

#include "CharacterBase.h"
#include "PlayerAbilitySystemComponent.h"
#include "PlayerAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InventoryComponent.h"
// TODO: This needs refactoring so we don't include loads of singular gameplay effects.
#include "BaseStaminaRecovery.h"
#include "SprintEffect.h"
#include "SprintCost.h"
#include "SneakEffect.h"
#include "BaseHealthRecovery.h"
#include "SetDefaultAttributes.h"
#include "GAJump.h"
#include "GameplayEffectTypes.h"
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

	if (isSprinting && getStamina() <= 0)
	{
		Logger::addMessage(TEXT("ACharacterBase::Tick(): Stamina depleted, stopping sprint"), SEVERITY::Info);
		setSprinting(false);
	}
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (abilitySystemComponent)
	{
		// When there is a change to the MoveSpeed in our AttributeSet, 
		// we want to update the MaxWalkSpeed of our CharacterMovementComponent accordingly.
		abilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPlayerAttributeSet::GetMoveSpeedAttribute())
			.AddLambda([this](const FOnAttributeChangeData& data) {
				GetCharacterMovement()->MaxWalkSpeed = data.NewValue;
			});
	}
	else
	{
		Logger::addMessage(TEXT("ACharacterBase::BeginPlay(): Failed to find AbilitySystemComponent, cannot bind MoveSpeed change delegate"), SEVERITY::Error);
	}

	if (attributeSet)
	{
		GetCharacterMovement()->MaxWalkSpeed = attributeSet->GetMoveSpeed();
	}
	else {Logger::addMessage(TEXT("ACharacterBase::BeginPlay(): Failed to find AttributeSet, cannot initialize MoveSpeed"), SEVERITY::Error);}
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
	//Instead of the built-in jump functionality, we want to trigger our GAJump ability.
	//ACharacter::Jump();
	
	if (abilitySystemComponent)
	{
		if (abilitySystemComponent->TryActivateAbilityByClass(UGAJump::StaticClass()))
		{
			//Logger::addMessage(TEXT("ACharacterBase::hasJumped(): Jump ability activated"), SEVERITY::Info);
		}
		else {Logger::addMessage(TEXT("ACharacterBase::hasJumped(): Failed to activate jump ability"), SEVERITY::Error);}
	}
}

/*
* Applies the Gameplay Effects for sprinting, and removes the base stamina recovery.
*/
void ACharacterBase::applySprintEffect()
{
	if (abilitySystemComponent)
	{
		FGameplayEffectContextHandle effectContext = abilitySystemComponent->MakeEffectContext();
		effectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle sprintSpec = abilitySystemComponent->MakeOutgoingSpec(USprintEffect::StaticClass(), 1, effectContext);
		abilitySystemComponent->ApplyGameplayEffectSpecToSelf(*sprintSpec.Data.Get());
		FGameplayEffectSpecHandle sprintCostSpec = abilitySystemComponent->MakeOutgoingSpec(USprintCost::StaticClass(), 1, effectContext);
		abilitySystemComponent->ApplyGameplayEffectSpecToSelf(*sprintCostSpec.Data.Get());

		abilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(UBaseStaminaRecovery::StaticClass(), nullptr, -1);
	}
	else {Logger::addMessage(TEXT("ACharacterBase::applySprintingEffect(): Failed to find AbilitySystemComponent"), SEVERITY::Error);}
}

/*
* Removes the Gameplay Effects for sprinting, and reapplies the base stamina recovery.
*/
void ACharacterBase::removeSprintEffect()
{
	if (abilitySystemComponent)
	{
		abilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(USprintEffect::StaticClass(), nullptr, -1);
		abilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(USprintCost::StaticClass(), nullptr, -1);

		FGameplayEffectContextHandle effectContext = abilitySystemComponent->MakeEffectContext();
		effectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle staminaRecoverySpec = abilitySystemComponent->MakeOutgoingSpec(UBaseStaminaRecovery::StaticClass(), 1, effectContext);
		abilitySystemComponent->ApplyGameplayEffectSpecToSelf(*staminaRecoverySpec.Data.Get());
	}
	else {Logger::addMessage(TEXT("ACharacterBase::removeSprintEffect(): Failed to find AbilitySystemComponent"), SEVERITY::Error);}
}

void ACharacterBase::applySneakEffect()
{
	if (abilitySystemComponent)
	{
		FGameplayEffectContextHandle effectContext = abilitySystemComponent->MakeEffectContext();
		effectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle sneakSpec = abilitySystemComponent->MakeOutgoingSpec(USneakEffect::StaticClass(), 1, effectContext);
		abilitySystemComponent->ApplyGameplayEffectSpecToSelf(*sneakSpec.Data.Get());
	}
	else {Logger::addMessage(TEXT("ACharacterBase::applySneakEffect(): Failed to find AbilitySystemComponent"), SEVERITY::Error);}
}

void ACharacterBase::removeSneakEffect()
{
	if (abilitySystemComponent)
	{
		abilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(USneakEffect::StaticClass(), nullptr, -1);
	}
	else {Logger::addMessage(TEXT("ACharacterBase::removeSneakEffect(): Failed to find AbilitySystemComponent"), SEVERITY::Error);}
}

/*
* Not sure if we saved anything by refactoring sprint/sneak into separate gameplay effects...
*    But it seems to be best-practice, and what you should do with GAS.
* 
* TODO: We should probably use the gameplay tags IsSprinting and IsSneaking instead of the booleans.
*			And also maybe make those tags mutually exclusive, maybe using GrantedApplicationImmunityTags or UImmunityGameplayEffectComponent?
*/
void ACharacterBase::setSprinting(const bool newIsSprinting)
{
	if (newIsSprinting)
	{
		// Avoid applying the effect multiple times if already sprinting.
		// Edge cases (like networking or combined state changes) can cause multiple applications.
		if (isSprinting) return;

		// Only allow sprinting if we have enough stamina
		if (getStamina() < 10)
		{
			Logger::addMessage(TEXT("ACharacterBase::setSprinting(): Not enough stamina to start sprinting"), SEVERITY::Info);
			return;
		}
		applySprintEffect();
		isSprinting = true;
		return;
	}
	
	// Take care of scenario where player is holding shift to sprint,
	//	then presses ctrl to sneak while still holding shift,
	//	and then lets go off shift, calling setSprinting(false).
	//		We do not want MaxWalkSpeed to be set to our walk speed
	isSprinting = false;
	if (isSneaking)
	{
		// If we stop sprinting but still hold sneak button, resume sneaking.
		removeSprintEffect();
		applySneakEffect();
		return;
	}

	removeSprintEffect();
	return;		// Not needed of course, but it makes it clear that nothing was accidently removed from this function
}

// TODO: We need better state handling for all this, use gameplay tags better.
void ACharacterBase::setSneaking(const bool newIsSneaking)
{
	if (newIsSneaking)
	{
		if (isSneaking) return;

		applySneakEffect();
		isSneaking = true;
		return;
	}

	// Same as the special scenario described in setSprinting()
	//		We do not want MaxWalkSpeed to be set to our walk speed
	//	TODO: For the future, this could be a sprint and slide functionality to add
	isSneaking = false;
	if (isSprinting)
	{
		// If we stop sneaking but still hold sprint button, resume sprinting.
		applySprintEffect();
		return;
	}
	//TODO: Probably need another check for if we are jumping. Currently we are slowing down mid-air if we "sneak".
	removeSneakEffect();
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
	else {Logger::addMessage(TEXT("ACharacterBase::getCharacterLevel(): AttributeSet not yet initialized"), SEVERITY::Error);}
	return 0;
}

float ACharacterBase::getHealth() const
{
	if (attributeSet)
	{
		return attributeSet->GetHealth();
	}
	else {Logger::addMessage(TEXT("ACharacterBase::getHealth(): AttributeSet not yet initialized"), SEVERITY::Error);}
	return 0.0f;
}

float ACharacterBase::getMaxHealth() const
{
	if (attributeSet)
	{
		return attributeSet->GetMaxHealth();
	}
	else {Logger::addMessage(TEXT("ACharacterBase::getMaxHealth(): AttributeSet not yet initialized"), SEVERITY::Error);}
	return 0.0f;
}

float ACharacterBase::getStamina() const
{
	if (attributeSet)
	{
		return attributeSet->GetStamina();
	}
	else {Logger::addMessage(TEXT("ACharacterBase::getStamina(): AttributeSet not yet initialized"), SEVERITY::Error);}
	return 0.0f;
}

float ACharacterBase::getMaxStamina() const
{
	if (attributeSet)
	{
		return attributeSet->GetMaxStamina();
	}
	else {Logger::addMessage(TEXT("ACharacterBase::getMaxStamina(): AttributeSet not yet initialized"), SEVERITY::Error);}
	return 0.0f;
}

float ACharacterBase::getStrength() const
{
	if (attributeSet)
	{
		return attributeSet->GetStrength();
	}
	else {Logger::addMessage(TEXT("ACharacterBase::getStrength(): AttributeSet not yet initialized"), SEVERITY::Error);}
	return 0.0f;
}

float ACharacterBase::getMaxStrength() const
{
	if (attributeSet)
	{
		return attributeSet->GetMaxStrength();
	}
	else {Logger::addMessage(TEXT("ACharacterBase::getMaxStrength(): AttributeSet not yet initialized"), SEVERITY::Error);}
	return 0.0f;
}

float ACharacterBase::getMoveSpeed() const
{
	if (attributeSet)
	{
		return attributeSet->GetMoveSpeed();
	}
	else {Logger::addMessage(TEXT("ACharacterBase::getMoveSpeed(): AttributeSet not yet initialized"), SEVERITY::Error);}
	return 0.0f;
}
float ACharacterBase::getSprintSpeed() const
{
	if (attributeSet)
	{
		return attributeSet->GetSprintSpeed();
	}
	else { Logger::addMessage(TEXT("ACharacterBase::getSprintSpeed(): AttributeSet not yet initialized"), SEVERITY::Error); }
	return 0.0f;
}
float ACharacterBase::getSneakSpeed() const
{
	if (attributeSet)
	{
		return attributeSet->GetSneakSpeed();
	}
	else { Logger::addMessage(TEXT("ACharacterBase::getSneakSpeed(): AttributeSet not yet initialized"), SEVERITY::Error); }
	return 0.0f;
}
float ACharacterBase::getNoise() const
{
	if (attributeSet)
	{
		return attributeSet->GetNoise();
	}
	else { Logger::addMessage(TEXT("ACharacterBase::getNoise(): AttributeSet not yet initialized"), SEVERITY::Error); }
	return 0.0f;
}
#pragma endregion

/*
* Apply the list of starting effects to the character once at startup.
*/
void ACharacterBase::initStartupEffects()
{
	startupEffects.AddUnique(UBaseStaminaRecovery::StaticClass());
	startupEffects.AddUnique(UBaseHealthRecovery::StaticClass());
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
* TODO: We should maybe move giving and removing abilities to the PlayerState.
* 
* Since abilities have animations and stuff, 
* it is probably easier to create them in the editor as Gameplay Ability blueprints, 
* and then add those blueprints to the defaultAbilities array in the editor.
*/
void ACharacterBase::initDefaultAbilities()
{
	defaultAbilities.AddUnique(UGAJump::StaticClass());
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
				// Cast to the base class to get the Input Id
				// TODO: Not sure if the inputId is actually needed.
				const UGameplayAbilityBase* abilityCDO = Cast<UGameplayAbilityBase>(startupAbility->GetDefaultObject());
				int32 inputId = abilityCDO ? static_cast<int32>(abilityCDO->abilityInputId) : -1;

				// Create a gameplay ability at level 1 and use the ASC to give that to the actor.
				const FGameplayAbilitySpec abilitySpec(startupAbility, 1, inputId);
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
			abilitySystemComponent->areDefaultAbilitiesGiven = true;
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
