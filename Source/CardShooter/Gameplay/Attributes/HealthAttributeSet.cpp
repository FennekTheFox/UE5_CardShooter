#include "Gameplay/Attributes/HealthAttributeSet.h"

#include "Net/UnrealNetwork.h"

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

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

void UHealthAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Health, OldValue);
}

void UHealthAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, MaxHealth, OldValue);
}

void UHealthAttributeSet::OnRep_TotalCurrentShieldAmount(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, TotalCurrentShieldAmount, OldValue);
}
