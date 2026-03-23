// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeWidget.generated.h"

/**
 * 
 */
UCLASS()
class UI_API UAttributeWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

protected:
	//virtual void NativeConstruct() override;

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
