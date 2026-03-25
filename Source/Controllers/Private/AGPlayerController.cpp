// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "AGPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "PlayerCharacter.h"
#include "Logger.h"

AAGPlayerController::AAGPlayerController()
{
	Logger::addMessage(TEXT("********** START OF LOGS **********"), SEVERITY::Debug, true, true, false);
	Logger::addMessage(TEXT("AAGPlayerController::AAGPlayerController(): Constructing new AAGPlayerController"), SEVERITY::Debug, true, true, false);
}

void AAGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Doing this in constructor causes nullptr crashing. Knowing the order of what is created when is a nightmare....
	PlayerCameraManager->ViewPitchMin = -80.0f;
	PlayerCameraManager->ViewPitchMax = 70.0f;
}

void AAGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		if (IsLocalPlayerController())
		{
			subsystem->ClearAllMappings();
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
		else
		{
			Logger::addMessage(TEXT("AAGPlayerController::SetupPlayerInputComponent(): Not a local player controller"), SEVERITY::Warning);
		}
	}
	else
	{
		Logger::addMessage(TEXT("AAGPlayerController::SetupPlayerInputComponent(): Failed finding Subsystem"), SEVERITY::Error);
	}
	
	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// MOVEMENT CONTROLS
		enhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Started, this, &AAGPlayerController::playerJump);
		enhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &AAGPlayerController::stopJump);
		enhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &AAGPlayerController::move);
		enhancedInputComponent->BindAction(sprintAction, ETriggerEvent::Started, this, &AAGPlayerController::sprintOn);
		enhancedInputComponent->BindAction(sprintAction, ETriggerEvent::Completed, this, &AAGPlayerController::sprintOff);
		enhancedInputComponent->BindAction(sneakAction, ETriggerEvent::Started, this, &AAGPlayerController::sneakOn);
		enhancedInputComponent->BindAction(sneakAction, ETriggerEvent::Completed, this, &AAGPlayerController::sneakOff);

		// CAMERA CONTROLS
		enhancedInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &AAGPlayerController::look);
		enhancedInputComponent->BindAction(mouseLookAction, ETriggerEvent::Triggered, this, &AAGPlayerController::look);
		enhancedInputComponent->BindAction(toggleCameraPerspective, ETriggerEvent::Started, this, &AAGPlayerController::togglePerspective);
	}
	else
	{
		Logger::addMessage(TEXT("AAGPlayerController::SetupPlayerInputComponent(): Failed finding an Enhanced Input Component"), SEVERITY::Error);
	}
}

// Tried moving PC to a class variable and initialize in constructor/BeginPlay, but that caused nullptr crashing.
void AAGPlayerController::move(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();
	APlayerCharacter* PC = CastChecked<APlayerCharacter>(GetPawn());
	PC->move(movementVector);
}

void AAGPlayerController::sprintOn()
{
	APlayerCharacter* PC = CastChecked<APlayerCharacter>(GetPawn());
	PC->sprintOn();
}

void AAGPlayerController::sprintOff()
{
	APlayerCharacter* PC = CastChecked<APlayerCharacter>(GetPawn());
	PC->sprintOff();
}

void AAGPlayerController::sneakOn()
{
	APlayerCharacter* PC = CastChecked<APlayerCharacter>(GetPawn());
	PC->sneakOn();
}

void AAGPlayerController::sneakOff()
{
	APlayerCharacter* PC = CastChecked<APlayerCharacter>(GetPawn());
	PC->sneakOff();
}

void AAGPlayerController::playerJump()
{
	APlayerCharacter* PC = CastChecked<APlayerCharacter>(GetPawn());
	PC->Jump();
}

void AAGPlayerController::stopJump()
{
	APlayerCharacter* PC = CastChecked<APlayerCharacter>(GetPawn());
	PC->StopJumping();
}

void AAGPlayerController::look(const FInputActionValue& value)
{
	FVector2D lookAxisVector = value.Get<FVector2D>();
	APlayerCharacter* PC = CastChecked<APlayerCharacter>(GetPawn());
	PC->look(lookAxisVector);
}

void AAGPlayerController::togglePerspective()
{
	APlayerCharacter* PC = CastChecked<APlayerCharacter>(GetPawn());
	PC->togglePerspective();
}
