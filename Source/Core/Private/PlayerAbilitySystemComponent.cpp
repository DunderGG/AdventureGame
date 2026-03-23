// Copyright (C) 2026 dunder.gg [GNU GPLv3]


#include "PlayerAbilitySystemComponent.h"
#include "Logger.h"

UPlayerAbilitySystemComponent::UPlayerAbilitySystemComponent()
{
	Logger::addMessage(TEXT("UPlayerAbilitySystemComponent::UPlayerAbilitySystemComponent(): Constructing new UPlayerAbilitySystemComponent"), SEVERITY::Info);
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UPlayerAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}
