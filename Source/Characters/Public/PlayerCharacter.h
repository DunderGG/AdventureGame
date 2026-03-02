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
UCLASS(Blueprintable, config=Game)
class CHARACTERS_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

private:
	void initAbilitySystemComponent();
	void initHUD() const;

#pragma region MOVEMENT
	void move(const FInputActionValue& value);
	void sprintOn();
	void sprintOff();
	void sneakOn();
	void sneakOff();
	void playerJump();
#pragma endregion	
#pragma region METAHUMAN
	void setupMetahuman();
	UPROPERTY(EditAnywhere)
	bool isUsingMetahuman = true;
	/*
	* THESE ARE POINTLESS WHEN WE USE bUsePawnControlRotation
	* and I cannot get it working without it, so... Here we are.
	*/
	UPROPERTY(EditAnywhere)
	float metaPitch = 0.0f;
	UPROPERTY(EditAnywhere)
	float metaYaw = -90.0f;
	UPROPERTY(EditAnywhere)
	float metaRoll = 0.0f;
#pragma endregion
#pragma region CAMERA
	void look(const FInputActionValue& value);
	void togglePerspective();
	void setToFirstPerson();
	void setToThirdPerson();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> cameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> thirdPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> firstPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool isInFirstPersonView;
#pragma endregion

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE TObjectPtr<USpringArmComponent> getCameraBoom() const { return cameraBoom; }
	FORCEINLINE TObjectPtr<UCameraComponent> getThirdPersonCamera() const { return thirdPersonCamera; }
	FORCEINLINE TObjectPtr<UCameraComponent> getFirstPersonCamera() const { return firstPersonCamera; }

	APlayerCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

	/*
	* GAS stuff
	* TODO: figure out what these do.
	* Supposedly meant to initialise the ASC on the client.
	*/
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Make the PlayerController a friend so we can keep movement/look functions private.
	friend class AAGPlayerController;
};
