#include "StatusEffectAttributeSet.h"

#include "Net/UnrealNetwork.h"

void UStatusEffectAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UStatusEffectAttributeSet, ShockBuildUp, COND_None, REPNOTIFY_Always);
}

void UStatusEffectAttributeSet::OnRep_ShockBuildUp(const FGameplayAttributeData& OldValue)
{
    UE_LOG(LogTemp, Log,
        TEXT("BEFORE REPNOTIFY: Property=%f"),
        ShockBuildUp.GetCurrentValue());

    if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
    {
        UE_LOG(LogTemp, Log,
            TEXT("BEFORE REPNOTIFY: ASC Current=%f Base=%f"),
            ASC->GetNumericAttribute(GetShockBuildUpAttribute()),
            ASC->GetNumericAttributeBase(GetShockBuildUpAttribute()));
    }

    GAMEPLAYATTRIBUTE_REPNOTIFY(UStatusEffectAttributeSet, ShockBuildUp, OldValue);

    if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
    {
        UE_LOG(LogTemp, Log,
            TEXT("AFTER REPNOTIFY: Property=%f ASC Current=%f Base=%f"),
            ShockBuildUp.GetCurrentValue(),
            ASC->GetNumericAttribute(GetShockBuildUpAttribute()),
            ASC->GetNumericAttributeBase(GetShockBuildUpAttribute()));
    }	
}

void UStatusEffectAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
    if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
    {
        UE_LOG(LogTemp, Log,
            TEXT("PreAttributeChanged: Current=%f Base=%f"),
            ASC->GetNumericAttribute(GetShockBuildUpAttribute()),
            ASC->GetNumericAttributeBase(GetShockBuildUpAttribute()));
    }
}

void UStatusEffectAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
    if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
    {
        UE_LOG(LogTemp, Log,
            TEXT("PostAttributeChanged: Current=%f Base=%f"),
            ASC->GetNumericAttribute(GetShockBuildUpAttribute()),
            ASC->GetNumericAttributeBase(GetShockBuildUpAttribute()));
    }
}
