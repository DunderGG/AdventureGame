// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AGPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class CONTROLLERS_API AAGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	// MOVEMENT
	void move(const FInputActionValue& value);
	void sprintOn();
	void sprintOff();
	void sneakOn();
	void sneakOff();
	void playerJump();
	void stopJump();

	// CAMERA
	void look(const FInputActionValue& value);
	void togglePerspective();

	/** Input Mapping Contexts 
	* This is filled in with the editor in the BP_PlayerController, so we don't have hardcoded references to them in code.
	*/
	UPROPERTY(EditAnywhere, Category = "Input | Input Mappings", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

#pragma region Input
	// MOVEMENT CONTROLS
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

protected:
	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

public:
	AAGPlayerController();
};
