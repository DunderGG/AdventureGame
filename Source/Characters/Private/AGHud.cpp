

#include "AGHud.h"
#include "AttributesWidget.h"
#include "CompassWidget.h"
#include "Logger.h"

AAGHud::AAGHud()
{
	UE_LOG(LogTemp, Display, TEXT("5. AAGHud::AAGHud(): Constructing new AAGHud"));
}

void AAGHud::init()
{
	attributesWidget = CreateWidget<UAttributesWidget>(GetOwningPlayerController(), attributesWidgetClass);
	if (attributesWidget)
	{
		attributesWidget->bindToAttributes();
		attributesWidget->AddToViewport();
		Logger::addMessage("AAGHud::init(): Successfully created AttributesWidget.");
	}
	else
	{
		Logger::addMessage("AAGHud::init(): Failed to create AttributesWidget.");
	}

	compassWidget = CreateWidget<UCompassWidget>(GetOwningPlayerController(), compassWidgetClass);
	if (compassWidget)
	{
		compassWidget->AddToViewport();
		Logger::addMessage("AAGHud::init(): Successfully created CompassWidget.");
	}
	else
	{
		Logger::addMessage("AAGHud::init(): Failed to create CompassWidget.");
	}
}
