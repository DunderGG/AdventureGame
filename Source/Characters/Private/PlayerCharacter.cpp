// Copyright dunder.gg. All Rights Reserved.

#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "AGAbilitySystemComponent.h"
#include "AGPlayerState.h"
#include "AGHud.h"
#include "AdventureGameplayTags.h"
#include "Logger.h"

APlayerCharacter::APlayerCharacter()
{
	UE_LOG(LogTemp, Display, TEXT("10. APlayerCharacter::APlayerCharacter(): Constructing new APlayerCharacter"));

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	GetCapsuleComponent()->InitCapsuleSize(45.0f, 100.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, rotationRate, 0.0f);
	GetCharacterMovement()->JumpZVelocity = jumpZVelocity;
	GetCharacterMovement()->AirControl = airControl;
	GetCharacterMovement()->MaxWalkSpeed = maxWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = minAnalogWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationFalling = brakingDecelerationFalling;
	GetCharacterMovement()->BrakingDecelerationWalking = brakingDecelerationWalking;

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("cameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	// TODO: Can we modify cameraBoom->TargetArmLength to make a zoom in and out effect when scrolling the mouse wheel?
	//			Dont forget to clamp value if we do that.
	cameraBoom->TargetArmLength = 400.0f;
	cameraBoom->bUsePawnControlRotation = true;

	thirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("thirdPersonCamera"));
	thirdPersonCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	thirdPersonCamera->bUsePawnControlRotation = false;
	thirdPersonCamera->bAutoActivate = false;

	firstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("firstPersonCamera"));
	firstPersonCamera->bUsePawnControlRotation = true;
	firstPersonCamera->bAutoActivate = false;
	
	/*
	* If using metahuman, we need to attach the first person camera to another component.
	*   GetMesh() works for "regular" characters though.
	*/
	if (!isUsingMetahuman)
	{
		firstPersonCamera->SetupAttachment(GetMesh(), "head");
		firstPersonCamera->SetRelativeLocation(FVector(-15, 20, 2.5));
		// Setting rotation rotates the camera component, but not actually the camera view, when we are using bUsePawnControlRotation = true
		firstPersonCamera->SetRelativeRotation(FRotator(0, -90, 90));
	}
	
	// Start in first or third person view?
	setToFirstPerson();
}

void APlayerCharacter::setToFirstPerson()
{
	// Set bOrientRotationToMovement to false in first person.
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	thirdPersonCamera->Deactivate();
	firstPersonCamera->Activate();
	isInFirstPersonView = true;
}
void APlayerCharacter::setToThirdPerson()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Character should not spin around in place when rotating camera.
	//   TODO: BUT maybe when having a weapon/tool equipped and holding down right click, this should be true.
	bUseControllerRotationYaw = false;
	firstPersonCamera->Deactivate();
	thirdPersonCamera->Activate();
	isInFirstPersonView = false;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (isUsingMetahuman)
	{
		setupMetahuman();
	}
}

void APlayerCharacter::setupMetahuman()
{
	/*
	*	Some real special handling just to set up the first person camera for the metahuman character.
	*     Must be some better way to do this...
	*/
	TArray<USkeletalMeshComponent*> skeletalMeshComponents;
	GetComponents<USkeletalMeshComponent>(skeletalMeshComponents, true);
	for (USkeletalMeshComponent* comp : skeletalMeshComponents)
	{
		if (comp->GetFName() == "Face")
		{
			firstPersonCamera->AttachToComponent(comp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "headSocket");
			firstPersonCamera->SetRelativeLocation(FVector(15, 20, 0));
			// Setting rotation rotates the camera component but not actually the camera view, when we are using bUsePawnControlRotation = true
			firstPersonCamera->SetRelativeRotation(FRotator(metaPitch, metaYaw, metaRoll));
			UE_LOG(LogTemp, Display, TEXT("APlayerCharacter::setupMetahuman(): Successfully setup the Metahuman"));
			break;
		}
	}
}

/*
* The default character class breaks this out into two functions,
*    where the DoMove function is BlueprintCallable.
*/
void APlayerCharacter::move(const FInputActionValue& value)
{
	FVector2D movementVector = value.Get<FVector2D>();

	// No need for GetController(), Characters already have a built-in Controller pointer.
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::move(): Failed finding Controller"));
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
		abilitySystemComponent->AddLooseGameplayTag(AdventureGameplayTags::Gameplay_State_IsInAir);
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
		AddControllerPitchInput(-lookAxisVector.Y);
	}
}

void APlayerCharacter::togglePerspective()
{
	isInFirstPersonView = !isInFirstPersonView;

	// Flip to third person camera
	if (!isInFirstPersonView)
	{
		setToThirdPerson();
		UE_LOG(LogTemp, Display, TEXT("APlayerCharacter::togglePerspective(): Now playing Third Person"));
		return;
	}

	// Flip to first person camera
	setToFirstPerson();
	
	UE_LOG(LogTemp, Display, TEXT("APlayerCharacter::togglePerspective(): Now playing First Person"));
	return;		// Not necessary, but makes it clear that the function ends here and nothing was accidentaly removed.
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){/* This is done in AGPlayerController now. */ }

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
	UE_LOG(LogTemp, Display, TEXT("APlayerCharacter::initAbilitySystemComponent(): Finished"));
}

/*
* HUD stuff
*/
void APlayerCharacter::initHUD() const
{
	if (const APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		// I assume this GetHUD() returns the HUD we told the GameMode blueprint to use,
		//   which is BP_DefaultHUD which inherits from AGHud.
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
	UE_LOG(LogTemp, Display, TEXT("APlayerCharacter::initHUD(): Finished"));
}

/*
* TODO: Dunno what this does...
* Initialises the ASC and default abilities on the server???
*/
void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	initAbilitySystemComponent();
	initDefaultAttributes();
	initStartupEffects();

	// Grant abilities and effects ONLY on the server
	// The granted abilities and gameplay effects should automatically replicate to the client.
	// We can see that this works by changing the default attributes in SetDefaultAttributes.cpp
	if (HasAuthority())
	{
		giveDefaultAbilities();
		giveDefaultAttributes();
		giveStartupEffects();
	}

	initHUD();

	UE_LOG(LogTemp, Display, TEXT("APlayerCharacter::PossessedBy(): Finished"));
}
/*
* TODO: Dunno what this is for...
* Initialises the ASC on the client???
*/
void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Client only needs to initialize the ASC info and HUD
	initAbilitySystemComponent();
	initHUD();

	UE_LOG(LogTemp, Display, TEXT("APlayerCharacter::OnRep_PlayerState(): Finished"));
}