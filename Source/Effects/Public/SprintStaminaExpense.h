// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SprintStaminaExpense.generated.h"

/**
 * 
 */
UCLASS()
class EFFECTS_API USprintStaminaExpense : public UGameplayEffect
{
	GENERATED_BODY()

private:
	virtual void PostInitProperties() override;
protected:
	// TODO: Not sure what UPROPERTY specifiers I need here, or if I need any at all.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS | Expense")
	float expenseMagnitude = -1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS | Expense")
	float expensePeriod = 0.1f;
public:
	
};
