// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly)
	float healthPercent;
	UPROPERTY(BlueprintReadOnly)
	float staminaPercent;

	UPROPERTY(BlueprintReadOnly)
	float health;
	UPROPERTY(BlueprintReadOnly)
	float stamina;

public:
	void bindToAttributes();
};
