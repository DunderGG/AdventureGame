// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "AttributesWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API UAttributesWidget : public UUserWidget
{
	GENERATED_BODY()

private:

protected:
	UPROPERTY(BlueprintReadOnly)
	float healthPercent;

	UPROPERTY(BlueprintReadOnly)
	float staminaPercent;

public:
	void bindToAttributes();
};
