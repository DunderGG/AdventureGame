// Copyright dunder.gg. All Rights Reserved.


#include "NPC.h"
#include "AGAbilitySystemComponent.h"
#include "AGCharacterAttributeSet.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;

	// Each character needs their own ASC
	abilitySystemComponent = CreateDefaultSubobject<UAGAbilitySystemComponent>(TEXT("abilitySystemComponent"));
	// TODO: Minimal means gameplay effects are neve replicated to anyone. Look into what this means?
	abilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	attributeSet = CreateDefaultSubobject<UAGCharacterAttributeSet>(TEXT("attributeSet"));
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
	abilitySystemComponent->InitAbilityActorInfo(this, this);
	// Give the default abilities as specified in the CharacterBase.
	giveDefaultAbilities();
	initDefaultAttributes();
}
