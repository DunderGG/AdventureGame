// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "AGPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "PlayerCharacter.h"

AAGPlayerController::AAGPlayerController()
{
	UE_LOG(LogTemp, Display, TEXT("1. AAGPlayerController::AAGPlayerController(): Constructing new AAGPlayerController"));
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
			//UE_LOG(LogTemp, Display, TEXT("AAGPlayerController::SetupPlayerInputComponent(): Is local player controller"));
			//UE_LOG(LogTemp, Display, TEXT("AAGPlayerController::SetupPlayerInputComponent(): Clearing mappings and adding the defaults"));
			subsystem->ClearAllMappings();
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				subsystem->AddMappingContext(CurrentContext, 0);
				//UE_LOG(LogTemp, Display, TEXT("AAGPlayerController::SetupPlayerInputComponent(): Added mapping context %s"), *CurrentContext->GetName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AAGPlayerController::SetupPlayerInputComponent(): Not a local player controller"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AAGPlayerController::SetupPlayerInputComponent(): Failed finding Subsystem"));
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
		UE_LOG(LogTemp, Error, TEXT("AAGPlayerController::SetupPlayerInputComponent(): Failed finding an Enhanced Input Component"));
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
