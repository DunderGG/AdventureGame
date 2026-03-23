#include "AGPlayerState.h"
#include "PlayerAbilitySystemComponent.h"
#include "PlayerAttributeSet.h"
#include "Logger.h"

AAGPlayerState::AAGPlayerState()
{
	Logger::addMessage(TEXT("AAGPlayerState::AAGPlayerState(): Constructing new AAGPlayerState"), SEVERITY::Info);

	// TODO: look into this. Player state update frequency = 100 times per second. The default is apparently too low and can cause a perceived lag when updating attributes?
	SetNetUpdateFrequency(100.0f);
	// Each character needs their own ASC
	abilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("abilitySystemComponent"));
	if (!abilitySystemComponent)
	{
		Logger::addMessage(TEXT("AAGPlayerState::AAGPlayerState(): Failed to create AbilitySystemComponent"), SEVERITY::Error);
		return;
	}
	// TODO: Mixed means gameplay effects are replicated to the owner of the character. Look into what this means?
	abilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	attributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("attributeSet"));	
	abilitySystemComponent->AddAttributeSetSubobject<UPlayerAttributeSet>(attributeSet);
}

UAbilitySystemComponent* AAGPlayerState::GetAbilitySystemComponent() const
{
	if (!abilitySystemComponent)
	{
		Logger::addMessage(TEXT("AAGPlayerState::GetAbilitySystemComponent(): Failed to find AbilitySystemComponent"), SEVERITY::Error);
	}
	// TODO: Should we use .Get like in CharacterBase or not?
	return abilitySystemComponent;
}

UPlayerAttributeSet* AAGPlayerState::GetAttributeSet() const
{
	return attributeSet;
}
