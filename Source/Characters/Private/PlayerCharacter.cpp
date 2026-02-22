// Copyright dunder.gg. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AGAbilitySystemComponent.h"
#include "AGPlayerState.h"
#include "AGCharacterAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "AGHud.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	GetCapsuleComponent()->InitCapsuleSize(45.0f, 100.0f);

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


	// Tried adding components to add the metahuman assets to,
	//	but it's probably easier to add the cameras and control to the
	//	metahuman character blueprint???
	// Create and attach all body components  -  Can't get stuff to line up...
	//body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("body"));
	//body->SetupAttachment(GetMesh());
	//face = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("face"));
	//face->SetupAttachment(GetMesh());
	//torso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("torso"));
	//torso->SetupAttachment(GetMesh());


	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("cameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->TargetArmLength = 400.0f;
	cameraBoom->bUsePawnControlRotation = true;

	thirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("thirdPersonCamera"));
	thirdPersonCamera->SetupAttachment(cameraBoom);
	thirdPersonCamera->bAutoActivate = false;

	firstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("firstPersonCamera"));
	firstPersonCamera->SetupAttachment(GetMesh(), "head");
	firstPersonCamera->SetRelativeRotation(FRotator(0, -90, 90));
	firstPersonCamera->SetRelativeLocation(FVector(15, 20, 2.5));
	firstPersonCamera->bUsePawnControlRotation = true;
	
	// Start in first or third person view?
	firstPersonCamera->Deactivate();
	thirdPersonCamera->Activate();
	isInFirstPersonView = false;

	// Widget
	hpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("hpBar"));
	hpBar->SetupAttachment(GetMesh());
	hpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	hpBar->SetWidgetSpace(EWidgetSpace::Screen);
	hpBar->SetDrawSize(FVector2D(150.0f, 20.f));
	hpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	/* Tried making a metahuman character, failed successfully...
	TArray<USkeletalMeshComponent*> skellies;
	GetComponents<USkeletalMeshComponent>(skellies, true);

	for (USkeletalMeshComponent* skelly : skellies)
	{
		if (skelly->GetFName() == "Face")
		{
			UE_LOG(LogTemp, Warning, TEXT("FOUND THE FACE"));
			firstPersonCamera->SetupAttachment(skelly, "head");
		}
	}
	*/
}

void APlayerCharacter::move(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// This way of adding movement removes head bobbing...
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
	return;		// Not necessary, but makes it clear that the function ends here
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


/*
* GAS stuff
*/
void APlayerCharacter::initAbilitySystemComponent()
{
	AAGPlayerState* playerState = GetPlayerState<AAGPlayerState>();
	if (playerState)
	{
		// TODO: Should we use CastChecked or have an if-else to not crash the game?
		abilitySystemComponent = CastChecked<UAGAbilitySystemComponent>(playerState->GetAbilitySystemComponent());

		abilitySystemComponent->InitAbilityActorInfo(playerState, this);

		// TOOD: Should this really be inside initASC()?
		attributeSet = playerState->GetAttributeSet();
	}
}
void APlayerCharacter::initHUD() const
{
	if (const APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (AAGHud* hud = Cast<AAGHud>(playerController->GetHUD()))
		{
			hud->init();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::initHUD(): Failed cast of HUD"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APlayerCharacter::initHUD(): Failed finding Controller"));
	}
}

/*
* TODO: Dunno what this does...
* Initialises the ASC and default abilities on the server???
*/
void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	initAbilitySystemComponent();

	// We only do this here and not in OnRep_PlayerState, because
	//   we only want to give abilities on the server???
	giveDefaultAbilities();
	giveDefaultAttributes();
	initHUD();
	giveStartupEffects();
}
/*
* TODO: Dunno what this is for...
* Initialises the ASC on the client???
*/
void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	initAbilitySystemComponent();
	giveDefaultAttributes();
	initHUD();
	giveStartupEffects();
}