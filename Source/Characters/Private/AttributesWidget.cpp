// Copyright dunder.gg. All Rights Reserved.


#include "AttributesWidget.h"
#include "AGCharacterAttributeSet.h"
#include "AGPlayerState.h"

/*
* The logic for informing our widget about attribute value changes.
*/
void UAttributesWidget::bindToAttributes()
{
	const AAGPlayerState* playerState = Cast<AAGPlayerState>(GetOwningPlayerState());
	if (playerState)
	{
		UAbilitySystemComponent* ASC = playerState->GetAbilitySystemComponent();
		const UAGCharacterAttributeSet* attributeSet = playerState->GetAttributeSet();

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
		UE_LOG(LogTemp, Error, TEXT("UAttributesWidget::bindToAttributes(): Failed cast of PlayerState"));
	}
}
