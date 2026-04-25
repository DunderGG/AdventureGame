#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AGPlayerState.generated.h"

class UPlayerAbilitySystemComponent;
class UPlayerAttributeSet;

UCLASS()
class COREDATA_API AAGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS")
	class UPlayerAbilitySystemComponent* abilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	TObjectPtr<UPlayerAttributeSet> attributeSet;

	// Probably not needed?
	//virtual void BeginPlay() override;

public:
	AAGPlayerState();

	// Implement IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UPlayerAttributeSet* GetAttributeSet() const;
};