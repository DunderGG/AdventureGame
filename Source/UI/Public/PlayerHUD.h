// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UAttributeWidget;
class UCompassWidget;

/**
 * 
 */
UCLASS()
class UI_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<UAttributeWidget> attributesWidget;

	UPROPERTY()
	TObjectPtr<UCompassWidget> compassWidget;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeWidget> attributesWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCompassWidget> compassWidgetClass;

public:
	APlayerHUD();
	void init();

};
