#include "AGAbilitySystemComponent.h"

UAGAbilitySystemComponent::UAGAbilitySystemComponent()
{
	UE_LOG(LogTemp, Display, TEXT("3. UAGAbilitySystemComponent::UAGAbilitySystemComponent(): Constructing new UAGAbilitySystemComponent"));
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UAGAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}
