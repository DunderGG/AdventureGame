

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AGHud.generated.h"

class UAttributesWidget;
class UCompassWidget;

/**
 * 
 */
UCLASS()
class CHARACTERS_API AAGHud : public AHUD
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<UAttributesWidget> attributesWidget;

	UPROPERTY()
	TObjectPtr<UCompassWidget> compassWidget;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributesWidget> attributesWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCompassWidget> compassWidgetClass;

public:
	AAGHud();
	void init();
	
	
};
