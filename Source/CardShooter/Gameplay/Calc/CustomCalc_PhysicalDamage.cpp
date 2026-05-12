#include "CustomCalc_PhysicalDamage.h"

#include "Gameplay/Attributes/DamageModifierAttributeSet.h"

UPhysicalDamageCalc::UPhysicalDamageCalc()
{
    DamageMultiplier = FGameplayEffectAttributeCaptureDefinition(
        UDamageModifierAttributeSet::GetDamageMultiplierAttribute(),
        EGameplayEffectAttributeCaptureSource::Source,
        false
    );
    RelevantAttributesToCapture.Add(DamageMultiplier);

    PhysicalDamageMultiplier = FGameplayEffectAttributeCaptureDefinition(
        UDamageModifierAttributeSet::GetPhysicalDamageMultiplierAttribute(),
        EGameplayEffectAttributeCaptureSource::Source,
        false
    );
    RelevantAttributesToCapture.Add(PhysicalDamageMultiplier);

    // Default SetByCaller tag
    SetByCallerTag = FGameplayTag::RequestGameplayTag(TEXT("Gameplay.Effect.Parameters.Damage"));
}

float UPhysicalDamageCalc::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    return 99.f;

    //float DamageMultiplierValue = 0.f;
    //float PhysicalDamageMultiplierValue = 0.f;

    //FAggregatorEvaluateParameters EvalParams;
    //GetCapturedAttributeMagnitude(DamageMultiplier, Spec, EvalParams, DamageMultiplierValue);
    //GetCapturedAttributeMagnitude(PhysicalDamageMultiplier, Spec, EvalParams, PhysicalDamageMultiplierValue);

    //const float SetByCallerValue =
    //    Spec.GetSetByCallerMagnitude(SetByCallerTag, false, 1.f);

    //return DamageMultiplierValue * PhysicalDamageMultiplierValue * SetByCallerValue;
}

