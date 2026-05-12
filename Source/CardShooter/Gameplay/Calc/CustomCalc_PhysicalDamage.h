#pragma once

#include "GameplayModMagnitudeCalculation.h"

#include "CustomCalc_PhysicalDamage.generated.h"

UCLASS()
class UPhysicalDamageCalc : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UPhysicalDamageCalc();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	FGameplayEffectAttributeCaptureDefinition DamageMultiplier;
	FGameplayEffectAttributeCaptureDefinition PhysicalDamageMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Calculation")
	FGameplayTag SetByCallerTag;
};