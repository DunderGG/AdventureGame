#include "AGPlayerState.h"
#include "AGAbilitySystemComponent.h"
#include "AGCharacterAttributeSet.h"

AAGPlayerState::AAGPlayerState()
{
	abilitySystemComponent = CreateDefaultSubobject<UAGAbilitySystemComponent>(TEXT("abilitySystemComponent"));

	characterAttributeSet = CreateDefaultSubobject<UAGCharacterAttributeSet>(TEXT("characterAttributeSet"));

	abilitySystemComponent->AddAttributeSetSubobject<UAGCharacterAttributeSet>(characterAttributeSet);
}

UAbilitySystemComponent* AAGPlayerState::GetAbilitySystemComponent() const
{
	return abilitySystemComponent;
}

void AAGPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AGPostInitializeComponents();
}

void AAGPlayerState::BeginPlay()
{
	Super::BeginPlay();
}