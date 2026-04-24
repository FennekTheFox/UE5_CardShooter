#include "HealthAttributeSet.h"

void UHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	//Super::PostAttributeChanged(Attribute, OldValue, NewValue);
	if (Attribute == GetHealthAttribute() || Attribute == GetMaxHealthAttribute()) 
	{
		if (GetHealth() > GetMaxHealth() || GetHealth() < 0.0f)
		{
			float ClampedValue = FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth());
			GetOwningAbilitySystemComponent()->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, ClampedValue);

		}
	}
}
