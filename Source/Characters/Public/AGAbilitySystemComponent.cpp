#include "AGAbilitySystemComponent.h"

UAGAbilitySystemComponent::UAGAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UAGAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}
