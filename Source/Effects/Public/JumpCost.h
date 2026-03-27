#include "PlayerAttributeSet.h"
#include "JumpCost.generated.h"
UCLASS()
class UJumpCost : public UGameplayEffect
{
    GENERATED_BODY()
public:
    UJumpCost()
    {
        DurationPolicy = EGameplayEffectDurationType::Instant;

        FGameplayModifierInfo Modifier;
        Modifier.Attribute = UPlayerAttributeSet::GetStaminaAttribute();
        Modifier.ModifierOp = EGameplayModOp::Additive;
        // Negative value to "cost" stamina
        Modifier.ModifierMagnitude = FScalableFloat(-10.0f);
        Modifiers.Add(Modifier);
    }
};
