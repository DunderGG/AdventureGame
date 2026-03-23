// Copyright (C) 2026 dunder.gg [GNU GPLv3]


#include "PlayerHUD.h"

#include "AttributeWidget.h"
#include "CompassWidget.h"
#include "Logger.h"

APlayerHUD::APlayerHUD()
{
	UE_LOG(LogTemp, Display, TEXT("APlayerHUD::APlayerHUD(): Constructing new APlayerHUD"));
}

void APlayerHUD::init()
{
	attributesWidget = CreateWidget<UAttributeWidget>(GetOwningPlayerController(), attributesWidgetClass);
	if (attributesWidget)
	{
		attributesWidget->bindToAttributes();
		attributesWidget->AddToViewport();
	}
	else
	{
		Logger::addMessage(TEXT("APlayerHUD::init(): Failed to create AttributesWidget."), SEVERITY::Error);
	}

	compassWidget = CreateWidget<UCompassWidget>(GetOwningPlayerController(), compassWidgetClass);
	if (compassWidget)
	{
		compassWidget->AddToViewport();
	}
	else
	{
		Logger::addMessage(TEXT("APlayerHUD::init(): Failed to create CompassWidget."), SEVERITY::Error);
	}
}
