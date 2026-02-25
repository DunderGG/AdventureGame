// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Effects/BaseStaminaRecovery.h"
#include "Effects/BaseHealthRecovery.h"
#include "Effects/SetDefaultAttributes.h"
#include "CharacterBase.generated.h"

class UGameplayAbility;
class UAGAbilitySystemComponent;
class UAGCharacterAttributeSet;
class UGameplayEffect;

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

#pragma region GAS stuff
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS | ASC")
	TObjectPtr<class UAGAbilitySystemComponent> abilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAGCharacterAttributeSet> attributeSet;

	// We put abilities into this array in the editor, using Gameplay Ability blueprints, like GA_Kick.
	//   TODO: This can't be const for some reason? I guess because we are setting the value in the editor?
	virtual void giveDefaultAbilities();
	UPROPERTY(EditDefaultsOnly, Category = "GAS | Ability")
	TArray<TSubclassOf<UGameplayAbility>> defaultAbilities;
	
	// Epic recommends to initialise attributes through a gameplay effect.
	UPROPERTY(EditDefaultsOnly, Category = "GAS | Attribute")
	TSubclassOf<UGameplayEffect> defaultAttributesEffect;
	virtual void giveDefaultAttributes();

	// A list of effects that we apply to the character once at startup.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS | Effect")
	TArray<TSubclassOf<class UGameplayEffect>> startupEffects;
	virtual void giveStartupEffects();

	virtual void initStartupEffects();
	virtual void initDefaultAttributes();
	//virtual void initDefaultAbilities();

#pragma endregion
public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Implement the GetAbilitySystemComponent function from the IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAGCharacterAttributeSet* GetAttributeSet() const;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region Attribute Set Getters
	UFUNCTION(BlueprintCallable, Category = "GAS | CharacterBase | Attributes")
	int32 getCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GAS | CharacterBase | Attributes")
	float getHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS | CharacterBase | Attributes")
	float getMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GAS | CharacterBase | Attributes")
	float getStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDCharacter|Attributes")
	float getMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GAS | CharacterBase | Attributes")
	float getStrength() const;

	UFUNCTION(BlueprintCallable, Category = "GAS | CharacterBase | Attributes")
	float getMaxStrength() const;
#pragma endregion
};
