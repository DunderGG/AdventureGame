

#include "AGHud.h"
#include "AttributesWidget.h"

AAGHud::AAGHud()
{
	UE_LOG(LogTemp, Display, TEXT("5. AAGHud::AAGHud(): Constructing new AAGHud"));
}

void AAGHud::init()
{
	attributesWidget = CreateWidget<UAttributesWidget>(GetOwningPlayerController(), attributesWidgetClass);
	attributesWidget->bindToAttributes();
	attributesWidget->AddToViewport();
}
