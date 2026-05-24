#pragma once

#include "CardAugment.h"
#include "NativeGameplayTags.h"

#include "CardAugment_ModifySetByCallerParameters.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Card_Augment);

UCLASS(Blueprintable)
class UCardAugmentNameGenerator_ModifyParameter : public UCardAugmentNameGenerator
{
	GENERATED_BODY()

public:
	FText GenerateName(const FCardAugmentData& ShardData) override;
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
class UCardAugment_ModifySetByCallerParameter : public UCardAugment
{
	GENERATED_BODY()

	UCardAugment_ModifySetByCallerParameter();

public:
	void ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const override;
};