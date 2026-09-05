#pragma once

#include "CardAugment.h"
#include "NativeGameplayTags.h"

#include "CardAugment_AddAttribute.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Card_Augment_AddAttribute);


UCLASS(Blueprintable)
class UCardAugmentDescriptionGenerator_AddAttribute : public UCardAugmentTextGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, FText> AttributeNames;

	FText GenerateText(const FCardAugmentData& AugmentData) const override;
};


UCLASS(Blueprintable)
class UCardAugmentNameGenerator_AddAttribute : public UCardAugmentTextGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, FText> AttributeAliases;

	FText GenerateText(const FCardAugmentData& AugmentData) const override;
};

UCLASS(Blueprintable)
class UCardAugmentShardNameGenerator_AddAttribute : public UCardAugmentShardTextGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, FText> AttributeAliases;

	FText Generate(const FCardAugmentShard& ShardData) override;
};

UCLASS(Blueprintable)
class UCardAugmentShardDescGenerator_AddAttribute : public UCardAugmentShardTextGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, FText> AttributeNames;

	FText Generate(const FCardAugmentShard& ShardData) override;
};


//Uses the Parameter.Magnitude to match damage types and change them to Parameter.VariantOption
UCLASS()
class UCardAugment_AddAttribute : public UCardAugment
{
	GENERATED_BODY()

	UCardAugment_AddAttribute();

public:
	void ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const override;
};