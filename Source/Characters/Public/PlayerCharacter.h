// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

// The classes we do want to create blueprints for need to be Blueprintable
UCLASS(Blueprintable)
class CHARACTERS_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

private:
	//UPROPERTY(EditAnywhere, Category = "Body Components")
	//TObjectPtr<USkeletalMeshComponent> body;
	//UPROPERTY(EditAnywhere, Category = "Body Components")
	//TObjectPtr<USkeletalMeshComponent> face;
	//UPROPERTY(EditAnywhere, Category = "Body Components")
	//TObjectPtr<USkeletalMeshComponent> torso;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> cameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> thirdPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> firstPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isInFirstPersonView;

protected:
	virtual void BeginPlay() override;

	// MOVEMENT
	void move(const FInputActionValue& value);
	void sprintOn();
	void sprintOff();
	void sneakOn();
	void sneakOff();
	void playerJump();

	// CAMERA
	void look(const FInputActionValue& value);
	void togglePerspective();

	// Pragma region lets us organise and hide code
#pragma region Input
	// MOVEMENT CONTROLS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> defaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> jumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> moveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> sprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> sneakAction;

	// CAMERA CONTROLS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> lookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> mouseLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> toggleCameraPerspective;
#pragma endregion

public:
	FORCEINLINE TObjectPtr<USpringArmComponent> getCameraBoom() const { return cameraBoom; }
	FORCEINLINE TObjectPtr<UCameraComponent> getThirdPersonCamera() const { return thirdPersonCamera; }
	FORCEINLINE TObjectPtr<UCameraComponent> getFirstPersonCamera() const { return firstPersonCamera; }

	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};
