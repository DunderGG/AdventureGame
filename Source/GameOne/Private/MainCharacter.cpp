// Copyright dunder.gg. All Rights Reserved.

#include "MainCharacter.h"
#include "EquippableToolBase.h"
#include "Data/EquippableToolDefinition.h"
#include "InventoryComponent.h"

AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//	If we turn it off, it still gets overridden, it seems...
	PrimaryActorTick.bCanEverTick = false;

	// COMPONENT CREATION
	// TODO  The imported SandboxCharacter already has a third person camera setup,
	//			but we should probably create this ourselves.
	// TODO  Maybe having an option for first person would be nice?
	//firstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//check(firstPersonCameraComponent != nullptr);

	// Create a first person mesh component for the owning player
	//firstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("firstPersonMesh"));
	//check(firstPersonMeshComponent != nullptr);
	// Attach the FirstPerson mesh to the Skeletal Mesh
	//firstPersonMeshComponent->SetupAttachment(GetMesh());

	// Create an Inventory Component for the owning player
	inventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("inventoryComponent"));

	//CAMERA SETUP

	// Attach the camera component to the first-person Skeletal Mesh
	//firstPersonCameraComponent->SetupAttachment(firstPersonMeshComponent, FName("head"));

	//firstPersonCameraComponent->SetRelativeLocationAndRotation(firstPersonCameraOffset, FRotator(0.0f, 90.0f, -90.0f));

	// Enable the pawn to control camera rotation
	//firstPersonCameraComponent->bUsePawnControlRotation = true;
		

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Only the owning player sees the first person mesh.
	//firstPersonMeshComponent->SetOnlyOwnerSee(true);

	// Set the animations on the first person mesh.
	//firstPersonMeshComponent->SetAnimInstanceClass(firstPersonDefaultAnim->GeneratedClass);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	// 2D Vector of movement values returned from the input action
	const FVector2D MovementValue = Value.Get<FVector2D>();

	// Check if the controller possessing this Actor is valid
	if (Controller)
	{
		// TODO  The imported character already has movement,
		//			but we need to fix this ourselves at some point
		// Add left and right movement
		const FVector Right = GetActorRightVector();
		//AddMovementInput(Right, MovementValue.X);

		// Add forward and back movement
		const FVector Forward = GetActorForwardVector();
		//AddMovementInput(Forward, MovementValue.Y);
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (Controller)
	{
		//  TODO  Same as for movement
		//AddControllerYawInput(LookAxisValue.X);
		//AddControllerPitchInput(LookAxisValue.Y);
	}
}

bool AMainCharacter::isToolAlreadyOwned(UEquippableToolDefinition* toolDefinition) const
{
	// Loop through the inventory to see if we already have that tool
	for (UEquippableToolDefinition* inventoryItem : inventoryComponent->toolInventory)
	{
		if (toolDefinition->id == inventoryItem->id)
		{
			UE_LOG(LogTemp, Display, TEXT("AMainCharacter::isToolAlreadyOwned(): Tool already in inventory"));
			return true;
		}
	}
	UE_LOG(LogTemp, Display, TEXT("AMainCharacter::isToolAlreadyOwned(): Found a new item: %s"), *toolDefinition->id.ToString());
	return false;
}

void AMainCharacter::attachTool(UEquippableToolDefinition* toolDefinition)
{
	// Only equip this tool if it isn't already owned
	//  TODO  This is a weird way to handle pickups, we should be able to have multiple of same item
	if (not isToolAlreadyOwned(toolDefinition))
	{
		/*
			The AEquippableToolBase tool stored inside ToolDefinition is an Actor, 
				so it may not be loaded when AttachTool() is called. 
				To handle this, you’re going to spawn a new instance of the tool using the SpawnActor() function.
			SpawnActor() is part of the UWorld object, 
				which is the core object that represents the map and the actors in it. 
				Access it by calling the GetWorld() function from any Actor.

				When you pass toolDefinition->toolAsset to SpawnActor, UE knows to look at toolAsset’s class type 
					and spawn that type of Actor. (toolAsset is the EquippableToolBase Actor associated with that toolDefinition.)
		*/
		
		// Spawn a new instance of the tool to equip
		AEquippableToolBase* toolToEquip = GetWorld()->SpawnActor<AEquippableToolBase>(toolDefinition->toolAsset, this->GetActorTransform());
	
		// Attach the tool to the Third Person Character ??????
		FAttachmentTransformRules attachmentRules(EAttachmentRule::SnapToTarget, true);

		toolToEquip->AttachToActor(this, attachmentRules);

		// Attach to the third person mesh, not first person
		// TODO  I assume if we want the option for first person camera, this would have to be fixed
		//toolToEquip->AttachToComponent(firstPersonMeshComponent, attachmentRules, FName(TEXT("HandGrip_R")));
		toolToEquip->AttachToComponent(GetMesh(), attachmentRules, FName(TEXT("hand_r")));
		
		/*
		*  A failed attempt at trying to rotate the pistol, since it is currently facing the wrong way
		* TODO  it's super ugly, this needs fixing
		*/
		//FTransform toolTransform = this->GetActorTransform();
		//toolTransform.SetComponents(FQuat(FVector(0.0f,0.0f,1.0f),FMath::DegreesToRadians(-90.0f)), toolTransform.GetTranslation(), FVector(1, 1, 1));
		//FQuat quat = GetActorQuat();
		//FQuat deltaQuat = FQuat(FVector(0.0f, 180.0f, 0.0f), FMath::DegreesToRadians(1.0f));
		//FQuat newQuat = quat + deltaQuat;
		//toolToEquip->SetActorRotation(newQuat);
		//toolToEquip->SetActorRotation(FRotator(7.016454, 106.772035, -26.193311));
		//toolToEquip->SetActorTransform(toolTransform);

		toolToEquip->owningCharacter = this;

		// Add the tool to the inventory
		inventoryComponent->toolInventory.Add(toolDefinition);
		
		/*
		* SetAnimInstanceClass dynamically changes the animation Blueprint at runtime for a skeletal mesh 
			and is commonly used when equipping items and weapons with different sets of animations. 
		  GeneratedClass gets the actual AnimInstance class generated from the Blueprint.

		  When we do this, we break the animations from the "Game Animation Sample" starter pack.
		  TODO fix animations, probably a big task...
		*/
		//firstPersonMeshComponent->SetAnimInstanceClass(toolToEquip->firstPersonToolAnim->GeneratedClass);
		GetMesh()->SetAnimInstanceClass(toolToEquip->thirdPersonToolAnim->GeneratedClass);

		equippedTool = toolToEquip;

		// Get the player controller for this character
		if (APlayerController* playerController = Cast <APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
			{
				/*
				* This time, when you add the tool’s Input Mapping Context to the player subsystem, 
					set the priority to 1. 
				  The priority of the player’s main mapping context (FirstPersonContext) is lower (0), 
					so when both mapping contexts have the same key binding, 
					the input binding in ToolToEquip->ToolMappingContext takes priority over FirstPersonContext. 
				*/
				subsystem->AddMappingContext(toolToEquip->toolMappingContext, 1);

				// Bind the character’s input action to the tool’s Use() function.
				toolToEquip->bindInputAction(useAction);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AMainCharacter::attachTool(): No local player subsystem found!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMainCharacter::attachTool(): No playerController found!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("AMainCharacter::attachTool(): Tool already owned"));
	}
}

void AMainCharacter::giveItem(UItemDefinition* itemDefinition)
{
	switch (itemDefinition->itemType)
	{
	case EItemType::Tool:
	{
		UEquippableToolDefinition* toolDefinition = Cast<UEquippableToolDefinition>(itemDefinition);
		if (toolDefinition != nullptr)
		{
			attachTool(toolDefinition);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AMainCharacter::giveItem(): Failed casting given item '%s'"), *itemDefinition->id.ToString());
		}

		UE_LOG(LogTemp, Display, TEXT("AMainCharacter::giveItem(): Picked up a tool: '%s'"), *itemDefinition->id.ToString());
		break;
	}
	case EItemType::Consumable:
	{
		//TODO fix consumables
		UE_LOG(LogTemp, Display, TEXT("AMainCharacter::giveItem(): Picked up a consumable: '%s'"), *itemDefinition->id.ToString());
		break;
	}
	default:
		break;
	}
}
/*
* To find the point that your character is looking at, you’re going to simulate a line trace 
*	along the vector the character is looking down to some distant point. 
*	If the line trace collides with an object, you know where in the world your character is looking.
*/
FVector AMainCharacter::getCameraTargetLocation()
{
	FVector targetResult(0,0,0);

	UWorld* const world = GetWorld();
	if (world != nullptr)
	{
		// The result of the line trace
		//	FHitResult is a struct in UE that stores information about the result of a collision query,
		//	including the Actor or Component that was hit and where you hit it.
		FHitResult hit;

		TArray<UCameraComponent*> cameras;
		this->GetComponents<UCameraComponent>(cameras);
		if (cameras.Num() == 0 && cameras[0] != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("AMainCharacter::getCameraTargetLocation(): Failed finding the camera"));
			return targetResult;
		}
		TObjectPtr<UCameraComponent> theCamera = cameras[0];
		const FVector traceStart = theCamera->GetComponentLocation();
		const FVector traceEnd = traceStart + theCamera->GetForwardVector() * 10000.0;

		//UE_LOG(LogTemp, Display, TEXT("AMainCharacter::getCameraTargetLocation(): camera location = %s"), *traceStart.ToString());
		/*
		* This simulates a line trace from TraceStart to TraceEnd, colliding with any visible objects 
		*	and storing the result of the trace in Hit. 
		* ECollisionChannel::ECC_Visibility is the channel to use for tracing, 
		*	and these channels define what types of objects your trace should try to hit. Use ECC_Visibility for line-of-sight camera checks.
		* 
		* The Hit value now contains information about the hit result, such as the location and normal of the impact. 
		*	It also knows if the hit was a result of an object collision. 
		*	The location of impact (or the end point of the trace line) is the camera target location to return. 
		*/
		world->LineTraceSingleByChannel(hit, traceStart, traceEnd, ECollisionChannel::ECC_Visibility);

		targetResult = hit.bBlockingHit ? hit.ImpactPoint : hit.TraceEnd;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AMainCharacter::getCameraTargetLocation(): Failed GetWorld"));
		return targetResult;
	}

	//UE_LOG(LogTemp, Display, TEXT("AMainCharacter::getCameraTargetLocation(): targetResult = %s"), *targetResult.ToString());
	return targetResult;
}