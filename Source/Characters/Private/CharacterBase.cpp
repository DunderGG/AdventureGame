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
{
	return abilitySystemComponent.Get();
}

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


