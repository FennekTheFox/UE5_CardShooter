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
	ATTRIBUTE_ACCESSORS(UStatusEffectAttributeSet, ImmolationBuildUp);
	ATTRIBUTE_ACCESSORS(UStatusEffectAttributeSet, ImmolationResistanceMultiplier);
	ATTRIBUTE_ACCESSORS(UStatusEffectAttributeSet, ImmolationAmplification);
	ATTRIBUTE_ACCESSORS(UStatusEffectAttributeSet, FreezeBuildUp);
	ATTRIBUTE_ACCESSORS(UStatusEffectAttributeSet, FreezeResistanceMultiplier);
	ATTRIBUTE_ACCESSORS(UStatusEffectAttributeSet, FreezeAmplification);

	UFUNCTION()
	void OnRep_ShockBuildUp(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_ImmolationBuildUp(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_FreezeBuildUp(const FGameplayAttributeData& OldValue);


	void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;


	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

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

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ImmolationBuildUp, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ImmolationBuildUp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ImmolationResistanceMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ImmolationAmplification;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FreezeBuildUp, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FreezeBuildUp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FreezeResistanceMultiplier;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FreezeAmplification;
};