#pragma once

#include "AbilitySystemComponent.h"
#include "ProjectAttributeSet.h"

#include "DamageModifierAttributeSet.generated.h"

UCLASS()
class UDamageModifierAttributeSet : public UProjectAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, DamageMultiplier);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, PhysicalDamageMultiplier);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, ElectricDamageMultiplier);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, ColdDamageMultiplier);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, PoisonDamageMultiplier);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, SanctusDamageMultiplier);

private:
	//Percent Damage Multipliers
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PhysicalDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ElectricDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ColdDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FireDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PoisonDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SanctusDamageMultiplier;
};