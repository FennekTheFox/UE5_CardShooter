#pragma once

#include "CardAugment.h"
#include "NativeGameplayTags.h"

#include "CardAugment_ParameterModifier.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Card_Augment_ParameterModifier);


USTRUCT(BlueprintType)
struct FModifierRange
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FFloatRange Range;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ESetByCallerMagnitudeDisplayType DisplayType = ESetByCallerMagnitudeDisplayType::Flat;
};

UCLASS(BlueprintType, Blueprintable)
class UMagnitudeCalculation_ParameterModifier : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, FModifierRange> AdditiveRanges;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, FModifierRange> MultiplicativeRanges;

	UFUNCTION()
	bool CalculateMagnitude(const FCardAugmentParameter& Parameter, float& OutMagnitude) const;
};

UCLASS(Blueprintable)
class UCardAugmentNameGenerator_ModifyParameter : public UCardAugmentTextGenerator
{
	GENERATED_BODY()

public:
	//Maps Magnitude alpha to Prefix
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<float, FText> PrefixMapping;

	//Maps scaling groups to their ftext suffixes
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, FText> SuffixMapping;

	FText GenerateText(const FCardAugmentData& AugmentData) const override;
};

UCLASS(Blueprintable)
class UCardAugmentDescriptionGenerator_ModifyParameter : public UCardAugmentTextGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, FText> ScalingGroupNames;

	FText GenerateText(const FCardAugmentData& AugmentData) const override;
};

UCLASS(Blueprintable)
class UCardAugmentShardNameGenerator_ModifyParameter : public UCardAugmentShardTextGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<float, FText> PrefixMapping;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, FText> SuffixMapping;

	FText Generate(const FCardAugmentShard& ShardData) override;
};

UCLASS(Blueprintable)
class UCardAugmentShardDescGenerator_ModifyParameter : public UCardAugmentShardTextGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, FText> ScalingGroupNames;

	FText Generate(const FCardAugmentShard& ShardData) override;
};

UENUM()
enum class EModifyOperationType : uint8
{
	Add = 0,
	Multiply = 1,
};

UCLASS()
class UCardAugment_ParameterModifier : public UCardAugment
{
	GENERATED_BODY()

	UCardAugment_ParameterModifier();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMagnitudeCalculation_ParameterModifier> MagnitudeCalculation = nullptr;

	void ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const override;
};