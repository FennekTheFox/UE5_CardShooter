#include "StatusEffectAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

void UStatusEffectAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UStatusEffectAttributeSet, ShockBuildUp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UStatusEffectAttributeSet, ImmolationBuildUp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UStatusEffectAttributeSet, FreezeBuildUp, COND_None, REPNOTIFY_Always);
}

void UStatusEffectAttributeSet::OnRep_ShockBuildUp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStatusEffectAttributeSet, ShockBuildUp, OldValue);
}

void UStatusEffectAttributeSet::OnRep_ImmolationBuildUp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStatusEffectAttributeSet, ImmolationBuildUp, OldValue);
}

void UStatusEffectAttributeSet::OnRep_FreezeBuildUp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStatusEffectAttributeSet, FreezeBuildUp, OldValue);
}

void UStatusEffectAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UStatusEffectAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetShockBuildUpAttribute() ||
		Attribute == GetImmolationBuildUpAttribute() ||
		Attribute == GetFreezeBuildUpAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 100.f);
	}
}

void UStatusEffectAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute& ModifiedAttribute = Data.EvaluatedData.Attribute;

	if (ModifiedAttribute == GetShockBuildUpAttribute())
	{
		SetShockBuildUp(FMath::Clamp(GetShockBuildUp(), 0.0f, 100.0f));
	}
	else if (ModifiedAttribute == GetFreezeBuildUpAttribute())
	{
		SetFreezeBuildUp(FMath::Clamp(GetFreezeBuildUp(), 0.0f, 100.0f));
	}
	else if (ModifiedAttribute == GetImmolationBuildUpAttribute())
	{
		SetImmolationBuildUp(FMath::Clamp(GetImmolationBuildUp(), 0.0f, 100.0f));
	}
}
