#pragma once

#include "ProjectAttributeSet.h"

#include "StatusEffectAttributeSet.generated.h"

UCLASS()
class UStatusEffectAttributeSet : public UProjectAttributeSet
{
	GENERATED_BODY()

public:
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(UStatusEffectAttributeSet, StatusResistanceMultiplier);
	ATTRIBUTE_ACCESSORS(UStatusEffectAttributeSet, StatusAmplification);
	ATTRIBUTE_ACCESSORS(UStatusEffectAttributeSet, ShockBuildUp);
	ATTRIBUTE_ACCESSORS(UStatusEffectAttributeSet, ShockResistanceMultiplier);
	ATTRIBUTE_ACCESSORS(UStatusEffectAttributeSet, ShockAmplification);

	UFUNCTION()
	void OnRep_ShockBuildUp(const FGameplayAttributeData& OldValue);


	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;


	void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StatusResistanceMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StatusAmplification;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShockBuildUp, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShockBuildUp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShockResistanceMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShockAmplification;
};