#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AGPlayerState.generated.h"

class UAGAbilitySystemComponent;
class UAGCharacterAttributeSet;

UCLASS()
class CHARACTERS_API AAGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS")
	class UAGAbilitySystemComponent* abilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	TObjectPtr<class UAGCharacterAttributeSet> attributeSet;

	// Probably not needed?
	//virtual void BeginPlay() override;

public:
	AAGPlayerState();

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAGCharacterAttributeSet* GetAttributeSet() const;
};