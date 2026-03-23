// Copyright (C) 2026 dunder.gg [GNU GPLv3]


#include "AttributeWidget.h"
#include "PlayerAttributeSet.h"
#include "AGPlayerState.h"
#include "Logger.h"

/*
* The logic for informing our widget about attribute value changes.
*/
void UAttributeWidget::bindToAttributes()
{
	const AAGPlayerState* playerState = Cast<AAGPlayerState>(GetOwningPlayerState());
	if (playerState)
	{
		UAbilitySystemComponent* ASC = playerState->GetAbilitySystemComponent();
		const UPlayerAttributeSet* attributeSet = playerState->GetAttributeSet();
		// Calculate our attribute values as percentages of maximum
		healthPercent = NUMERIC_VALUE(attributeSet, Health) / NUMERIC_VALUE(attributeSet, MaxHealth);
		staminaPercent = NUMERIC_VALUE(attributeSet, Stamina) / NUMERIC_VALUE(attributeSet, MaxStamina);
		health = NUMERIC_VALUE(attributeSet, Health);
		stamina = NUMERIC_VALUE(attributeSet, Stamina);
		// Bind delegates to update our attribute values whenever they change.
		ASC->GetGameplayAttributeValueChangeDelegate(attributeSet->GetHealthAttribute()).AddLambda(
			[this, attributeSet](const FOnAttributeChangeData& data)
			{
				healthPercent = data.NewValue / NUMERIC_VALUE(attributeSet, MaxHealth);
				health = data.NewValue;
			}
		);
		ASC->GetGameplayAttributeValueChangeDelegate(attributeSet->GetStaminaAttribute()).AddLambda(
			[this, attributeSet](const FOnAttributeChangeData& data)
			{
				staminaPercent = data.NewValue / NUMERIC_VALUE(attributeSet, MaxStamina);
				stamina = data.NewValue;
			}
		);
	}
	else
	{
		Logger::addMessage(TEXT("AttributeWidget: Failed to get player state."), SEVERITY::Error);
	}
}