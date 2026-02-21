

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AGHud.generated.h"

class UAttributesWidget;

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

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributesWidget> attributesWidgetClass;

public:
	void init();
	
	
};
