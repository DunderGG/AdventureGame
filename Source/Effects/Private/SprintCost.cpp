// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "SprintCost.h"
#include "PlayerAttributeSet.h"

void USprintCost::PostInitProperties()
{
	Super::PostInitProperties();

	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = costPeriod;
	bExecutePeriodicEffectOnApplication = true;

	FGameplayModifierInfo mod;
	mod.Attribute = UPlayerAttributeSet::GetStaminaAttribute();
	mod.ModifierOp = EGameplayModOp::Additive;
	mod.ModifierMagnitude = FScalableFloat(costMagnitude);
	Modifiers.Add(mod);
}
