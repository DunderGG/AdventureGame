// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "InputActionValue.h"
#include "MainCharacter.generated.h"

class AEquippableToolBase;
class UAnimBlueprint;
class UEquippableToolDefinition;
class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UInventoryComponent;
class UItemDefinition;

UCLASS()
class GAMEONE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Handles Look Input
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	// Handles 2D Movement Input
	UFUNCTION()
	void Move(const FInputActionValue& Value);


	// Attach and equip a tool to the player
	UFUNCTION()
	void attachTool(UEquippableToolDefinition* toolDefinition);

	// Returns the location in the world the character is looking at
	UFUNCTION()
	FVector getCameraTargetLocation() const;

	// Public function that other classes can call to attempt to give an item to the player
	UFUNCTION()
	void giveItem(UItemDefinition* itemDefinition);

	// Returns whether or not the player already owns this tool
	UFUNCTION()
	bool isToolAlreadyOwned(UEquippableToolDefinition* toolDefinition) const;

	// First Person camera
	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UCameraComponent> firstPersonCameraComponent;

	//UPROPERTY(EditAnywhere, Category = "Camera")
	//FVector firstPersonCameraOffset = FVector(2.8f, 5.9f, 0.0f);

	// First-person camera field of view
	//UPROPERTY(EditAnywhere, Category = "Camera")
	//float firstPersonFieldOfView = 70.0f;

	// First-person camera view scale
	//UPROPERTY(EditAnywhere, Category = "Camera")
	//float firstPersonViewScale = 0.6f;

	// Third Person Camera
	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UCameraComponent> thirdPersonCameraComponent;

	// First-person mesh, visible only to the owning player
	//UPROPERTY(VisibleAnywhere, Category = "Mesh")
	//TObjectPtr<USkeletalMeshComponent> firstPersonMeshComponent;

	// The Character Inventory Component
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TObjectPtr<UInventoryComponent> inventoryComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The currently equipped tool
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tools")
	TObjectPtr<AEquippableToolBase> equippedTool;

	// Jump Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> jumpAction;

	// Look Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> lookAction;

	// Move Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> moveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> thirdPersonContext;

	// This is the “use tool” input action that you’ll bind to the tool’s Use() function. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> useAction;

	// First Person animations
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	//UAnimBlueprint* firstPersonDefaultAnim;
};
