// Copyright dunder.gg. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("cameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->TargetArmLength = 400.0f;
	cameraBoom->bUsePawnControlRotation = true;

	thirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("thirdPersonCamera"));
	thirdPersonCamera->SetupAttachment(cameraBoom);
	thirdPersonCamera->Deactivate();
	thirdPersonCamera->bAutoActivate = false;

	firstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("firstPersonCamera"));
	firstPersonCamera->SetupAttachment(GetMesh(), "head");
	firstPersonCamera->SetRelativeRotation(FRotator(0, -90, 90));
	firstPersonCamera->SetRelativeLocation(FVector(15, 20, 2.5));
	firstPersonCamera->bUsePawnControlRotation = true;
	firstPersonCamera->Activate();

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::move(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// This way of adding movement removes head bobbing
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);
		const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		// While this way has a lot of head bobbing... What to use?
		//const FVector rightDirection = GetActorRightVector();
		//const FVector forwardDirection = GetActorForwardVector();
		
		AddMovementInput(forwardDirection, movementVector.Y);
		AddMovementInput(rightDirection, movementVector.X);
	}
}

void APlayerCharacter::sprintOn()
{
	Super::setSprinting(true);
}

void APlayerCharacter::sprintOff()
{
	Super::setSprinting(false);
}

void APlayerCharacter::sneakOn()
{
	Super::setSneaking(true);
}

void APlayerCharacter::sneakOff()
{
	Super::setSneaking(false);
}

void APlayerCharacter::playerJump()
{
	if (ACharacterBase::canCharacterJump() && !GetMovementComponent()->IsFalling())
	{
		ACharacterBase::hasJumped();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::playerJump(): Not allowed to jump"));
	}
}

void APlayerCharacter::look(const FInputActionValue& value)
{
	FVector2D lookAxisVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(lookAxisVector.X);
		AddControllerPitchInput(lookAxisVector.Y);
	}
}

void APlayerCharacter::togglePerspective()
{
	isInFirstPersonView = !isInFirstPersonView;

	// Flip to third person camera
	if (!isInFirstPersonView)
	{
		firstPersonCamera->Deactivate();
		thirdPersonCamera->Activate();

		bUseControllerRotationPitch = false;
		bUseControllerRotationYaw = false;
		bUseControllerRotationRoll = false;
		UE_LOG(LogTemp, Display, TEXT("APlayerCharacter::togglePerspective(): Now playing Third Person"));
		return;
	}

	// Flip to first person camera
	thirdPersonCamera->Deactivate();
	firstPersonCamera->Activate();

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
	UE_LOG(LogTemp, Display, TEXT("APlayerCharacter::togglePerspective(): Now playing First Person"));
	return;
}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(defaultMappingContext, 0);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::SetupPlayerInputComponent(): Failed finding Subsystem"));
		}

		if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			// MOVEMENT CONTROLS
			enhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Started, this, &APlayerCharacter::playerJump);
			enhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
			enhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::move);
			enhancedInputComponent->BindAction(sprintAction, ETriggerEvent::Started, this, &APlayerCharacter::sprintOn);
			enhancedInputComponent->BindAction(sprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::sprintOff);
			enhancedInputComponent->BindAction(sneakAction, ETriggerEvent::Started, this, &APlayerCharacter::sneakOn);
			enhancedInputComponent->BindAction(sneakAction, ETriggerEvent::Completed, this, &APlayerCharacter::sneakOff);

			// CAMERA CONTROLS
			enhancedInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::look);
			enhancedInputComponent->BindAction(mouseLookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::look);
			enhancedInputComponent->BindAction(toggleCameraPerspective, ETriggerEvent::Started, this, &APlayerCharacter::togglePerspective);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::SetupPlayerInputComponent(): Failed cast of Enhanced Input"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::SetupPlayerInputComponent(): Failed finding Controller"));
	}
}