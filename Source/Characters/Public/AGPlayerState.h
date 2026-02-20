#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "AGPlayerState.generated.h"

UCLASS()
class CHARACTERS_API AAGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAGPlayerState();

	// Implement IAbilitySystemInterface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PostInitializeComponents() override;


	// We can call this from blueprints, but not sure why...
	UFUNCTION(BlueprintImplementableEvent)
	void AGPostInitializeComponents();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS")
	class UAGAbilitySystemComponent* abilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	TObjectPtr<class UAGCharacterAttributeSet> characterAttributeSet;

	virtual void BeginPlay() override;
};