// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquippableToolBase.generated.h"

// The tool needs to know what Character it's equipped to 
// so it can bind tool-specific Input Actions to that Character
class AMainCharacter;
class UInputAction;
class UInputMappingContext;

// In the UCLASS macro, expose the class to Blueprints
UCLASS(BlueprintType, Blueprintable)
class GAMEONE_API AEquippableToolBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEquippableToolBase();

	// First Person animations
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//TObjectPtr<UAnimBlueprint> firstPersonToolAnim;

	// The Character holding the tool
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AMainCharacter> owningCharacter;

	// Third Person animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimBlueprint> thirdPersonToolAnim;

	// The IMC associated with this tool
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> toolMappingContext;

	// The Tool Skeletal Mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> toolMeshComponent;

	/**
	* When you implemented character movement controls, you used the InputComponent’s BindAction() function 
		that requires you to pass the exact name of the target function. 
		You don’t know the full name of the function yet, so you need a custom BindInputAction() function 
		that you can implement in each EquippableToolBase subclass to call BindAction, passing [ToolChildClass]::Use.
	*/
	// Binds the use function to the owning Character
	UFUNCTION()
	virtual void bindInputAction(const UInputAction* actionToBind);

	// Use the tool
	UFUNCTION()
	virtual void use();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
