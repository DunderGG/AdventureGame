// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "CharacterBase.generated.h"

// We make it abstract so we don't make new blueprints from the base class
UCLASS(Abstract, NotBlueprintable)
class CHARACTERS_API ACharacterBase : public ACharacter,
									  public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
#pragma region Movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float sneakSpeed = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float walkSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float sprintSpeed = 800.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool isSprinting= false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool isSneaking = false;
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region MovementFunctions
	bool canCharacterJump() const;
	void hasJumped();

	float getSneakSpeed() const;
	float getWalkSpeed() const;
	float getSprintSpeed() const;

	void setSprinting(const bool newIsSprinting);
	void setSneaking(const bool newIsSneaking);
#pragma endregion

	/*
	* GAS stuff
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<class UAGAbilitySystemComponent> abilitySystemComponent;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS | Weapon")
	TObjectPtr<class USkeletalMeshComponent> weaponMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS | UI")
	TObjectPtr<class UWidgetComponent> hpBar;

public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
