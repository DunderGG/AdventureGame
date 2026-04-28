// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AdventureGame/AdventureGame.h"
#include "CharacterBase.generated.h"

class UGameplayAbility;
class UPlayerAbilitySystemComponent;
class UPlayerAttributeSet;
class UGameplayEffect;
class UInventoryComponent;

// We make it abstract so we don't make new blueprints from the base class
UCLASS(Abstract, NotBlueprintable)
class CHARACTERS_API ACharacterBase : public ACharacter,
									  public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
#pragma region Movement
	// Expose these to the editor so we can play with the values without having to recompile.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float jumpZVelocity = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float airControl = 0.15f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float maxWalkSpeed = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float minAnalogWalkSpeed = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float brakingDecelerationFalling = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float brakingDecelerationWalking = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float rotationRate = 250.0f;
	
	//TODO These can be removed now that we are using tags i guess?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool isMoving = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool isAirborne = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool isSprinting = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool isSneaking = false;
#pragma endregion

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryComponent> inventoryComponent;

	void applyStaminaDepletion();
	void applyStaminaRecovery();

	void applySprintEffect();
	void removeSprintEffect();

	void applySneakEffect();
	void removeSneakEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


#pragma region MovementFunctions
	bool canCharacterJump() const;
	void hasJumped();

	float getMoveSpeed() const;
	float getSprintSpeed() const;
	float getSneakSpeed() const;

	float getNoise() const;

	void setSprinting(const bool newIsSprinting);
	void setSneaking(const bool newIsSneaking);
#pragma endregion

#pragma region GAS stuff
	// The PlayerCharacter will inherit this and initialise it in initAbilitySystemComponent() with 
	//   what the PlayerState contains.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS | ASC")
	TObjectPtr<UPlayerAbilitySystemComponent> abilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UPlayerAttributeSet> attributeSet;

	// We put abilities into this array in the editor, using Gameplay Ability blueprints, like GA_Kick.
	UPROPERTY(EditDefaultsOnly, Category = "GAS | Ability")
	TArray<TSubclassOf<UGameplayAbility>> defaultAbilities;
	virtual void giveDefaultAbilities();
	virtual void initDefaultAbilities();
	
	// Epic recommends to initialise attributes through a gameplay effect.
	UPROPERTY(EditDefaultsOnly, Category = "GAS | Attribute")
	TSubclassOf<UGameplayEffect> defaultAttributesEffect;
	virtual void applyDefaultAttributes();
	virtual void initDefaultAttributes();

	// A list of effects that we apply to the character once at startup.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS | Effect")
	TArray<TSubclassOf<UGameplayEffect>> startupEffects;
	virtual void applyStartupEffects();
	virtual void initStartupEffects();


#pragma endregion
public:
	// Sets default values for this character's properties
	ACharacterBase();

	virtual void Tick(float DeltaTime) override;

	// Implement the GetAbilitySystemComponent function from the IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UPlayerAttributeSet* GetAttributeSet() const;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	UInventoryComponent* getInventory() const;
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
