#include "AGPlayerState.h"
#include "AGAbilitySystemComponent.h"
#include "AGCharacterAttributeSet.h"

AAGPlayerState::AAGPlayerState()
{
	// TODO: look into this. Player state update frequency = 100 times per second. The default is apparently too low and can cause a perceived lag when updating attributes?
	SetNetUpdateFrequency(100.0f);
	// Each character needs their own ASC
	abilitySystemComponent = CreateDefaultSubobject<UAGAbilitySystemComponent>(TEXT("abilitySystemComponent"));
	// TODO: Mixed means gameplay effects are replicated to the owner of the character. Look into what this means?
	abilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	attributeSet = CreateDefaultSubobject<UAGCharacterAttributeSet>(TEXT("attributeSet"));

	abilitySystemComponent->AddAttributeSetSubobject<UAGCharacterAttributeSet>(attributeSet);
}

UAbilitySystemComponent* AAGPlayerState::GetAbilitySystemComponent() const
{
	// TODO: Should we use .Get like in CharacterBase or not?
	return abilitySystemComponent;
}

UAGCharacterAttributeSet* AAGPlayerState::GetAttributeSet() const
{
	return attributeSet;
}


/*
void AAGPlayerState::BeginPlay()
{
	Super::BeginPlay();
}
*/