// Copyright dunder.gg. All Rights Reserved.


#include "NPC.h"
#include "PlayerAbilitySystemComponent.h"
#include "PlayerAttributeSet.h"
#include "Logger.h"

ANPC::ANPC()
{
	Logger::addMessage(TEXT("ANPC::ANPC(): Constructing new ANPC"), SEVERITY::Debug, true, true, false);

	PrimaryActorTick.bCanEverTick = true;

	// Each character needs their own ASC
	// TODO: Change this to another type of ASC and Attribute set
	abilitySystemComponent = CreateDefaultSubobject<UPlayerAbilitySystemComponent>(TEXT("abilitySystemComponent"));
	// TODO: Minimal means gameplay effects are neve replicated to anyone. Look into what this means?
	abilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	attributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("attributeSet"));
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
	abilitySystemComponent->InitAbilityActorInfo(this, this);
	// Give the default abilities as specified in the CharacterBase.
	giveDefaultAbilities();
	giveDefaultAttributes();
}
