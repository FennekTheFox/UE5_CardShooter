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
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, LightningDamageMultiplier);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, ColdDamageMultiplier);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, PoisonDamageMultiplier);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, SanctusDamageMultiplier);


	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, DamageResistance);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, PhysicalDamageResistance);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, LightningDamageResistance);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, ColdDamageResistance);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, PoisonDamageResistance);
	ATTRIBUTE_ACCESSORS(UDamageModifierAttributeSet, SanctusDamageResistance);

private:
	//Outgoing percent Damage Multipliers
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PhysicalDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData LightningDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ColdDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FireDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PoisonDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SanctusDamageMultiplier;


	//Incoming percent Damage Multipliers
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DamageResistance;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PhysicalDamageResistance;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData LightningDamageResistance;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ColdDamageResistance;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FireDamageResistance;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData PoisonDamageResistance;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SanctusDamageResistance;
};