#pragma once

#include "GameplayEffectExtension.h"
#include "ProjectAbilitySystemComponent.h"
#include "AbilityTriggers.generated.h"

UCLASS(BlueprintType, Abstract, EditInlineNew, DefaultToInstanced, CollapseCategories)
class PROJECTABILITYSYSTEM_PLUGIN_API UAbilityActivationTrigger : public UObject
{
	GENERATED_BODY()

public:
	virtual FName GetTriggerName() { return NAME_None;};
};

// Attempts to activate the ability once right after it has been granted.
UCLASS(BlueprintType, DisplayName = "On Ability Granted")
class UAbilityActivationTrigger_OnAbilityGranted : public UAbilityActivationTrigger
{
	GENERATED_BODY()
public:
	FName GetTriggerName() override { return FName("OnAbilityGranted"); };
};

// Attempts to activate the ability each time its assigned Input Action triggers.
UCLASS(BlueprintType, DisplayName = "On Input Triggered")
class UAbilityActivationTrigger_OnInputTriggered : public UAbilityActivationTrigger
{
	GENERATED_BODY()

public:
	FName GetTriggerName() override { return FName("OnInputTriggered"); };

	// For how long the ability will keep trying to activate after it couldn't immediately be activated on input started.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float InputBufferDuration = 0.f;
};

// Attempts to activate the ability each time the specified Gameplay Tag gets added to its Owner.
UCLASS(BlueprintType, DisplayName = "On Tag Added")
class UAbilityActivationTrigger_OnTagAdded : public UAbilityActivationTrigger
{
	GENERATED_BODY()

public:
	FName GetTriggerName() override { return FName("OnTagAdded"); };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag GameplayTag;
};

// Attempts to activate the ability each time the specified Gameplay Tag gets added to its Owner, cancels it when the Tag gets removed.
UCLASS(BlueprintType, DisplayName = "While Tag Present")
class UAbilityActivationTrigger_WhileTagPresent : public UAbilityActivationTrigger
{
	GENERATED_BODY()

public:
	FName GetTriggerName() override { return FName("WhileTagPresent"); };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag GameplayTag;
};

// Attempts to activate the ability every tick while the specified Gameplay Tag is present on its Owner, cancels it when the Tag gets removed.
UCLASS(BlueprintType, DisplayName = "While Tag Present (Ongoing)")
class UAbilityActivationTrigger_WhileTagPresentOngoing : public UAbilityActivationTrigger
{
	GENERATED_BODY()

public:
	FName GetTriggerName() override { return FName("WhileTagPresentOngoing"); };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag GameplayTag;
};

// Attempts to activate the ability each time a Gameplay Event with the specified Gameplay Tag happens.
UCLASS(BlueprintType, DisplayName = "On Gameplay Event")
class UAbilityActivationTrigger_OnGameplayEvent : public UAbilityActivationTrigger
{
	GENERATED_BODY()

public:
	FName GetTriggerName() override { return FName("OnGameplayEvent"); };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag GameplayTag;
};

UENUM()
enum class EComparisonOperation : uint8
{
	LessThan,
	LessThanEq,
	Equals,
	GreaterThanEq,
	GreaterThan,
	NotEquals
};

// Attempts to activate the ability each time a Gameplay Event with the specified Gameplay Tag happens.
UCLASS(BlueprintType, DisplayName = "Attribute Based")
class UAbilityActivationTrigger_AttributeBased : public UAbilityActivationTrigger
{
	friend class UProjectAbilitySystemComponent;

	GENERATED_BODY()

public:
	FName GetTriggerName() override { return FName("Attribute Based"); };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayAttribute ReferenceAttribute;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EComparisonOperation ComparisonOperation = EComparisonOperation::Equals;

	//The right hand side value of the comparison operation
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ComparisonValue = 100.f;

	bool OnAttributeChanged(float NewValue) const
	{
		bool bSuccess = false;

		switch (ComparisonOperation)
		{
		case EComparisonOperation::Equals: bSuccess = FMath::IsNearlyEqual(NewValue, ComparisonValue); break;
		case EComparisonOperation::LessThan: bSuccess = NewValue < ComparisonValue; break;
		case EComparisonOperation::LessThanEq: bSuccess = NewValue <= ComparisonValue; break;
		case EComparisonOperation::GreaterThanEq: bSuccess = NewValue >= ComparisonValue; break;
		case EComparisonOperation::GreaterThan: bSuccess = NewValue > ComparisonValue; break;
		case EComparisonOperation::NotEquals: bSuccess = !FMath::IsNearlyEqual(NewValue, ComparisonValue); break;
		}

		return bSuccess;
	}
};
