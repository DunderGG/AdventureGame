

#include "AGHud.h"
#include "AttributesWidget.h"

void AAGHud::init()
{
	attributesWidget = CreateWidget<UAttributesWidget>(GetOwningPlayerController(), attributesWidgetClass);
	attributesWidget->bindToAttributes();
	attributesWidget->AddToViewport();
}
