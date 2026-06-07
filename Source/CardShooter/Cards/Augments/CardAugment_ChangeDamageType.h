#pragma once

#include "CardAugment.h"
#include "NativeGameplayTags.h"

#include "CardAugment_ChangeDamageType.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Card_Augment_ChangeDamageType);


UCLASS(Blueprintable)
class UCardAugmentDescriptionGenerator_ChangeDamageType : public UCardAugmentTextGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<int32, FText> DamageTypeNames;

	FText GenerateText(const FCardAugmentData& AugmentData) const override;
};


UCLASS(Blueprintable)
class UCardAugmentNameGenerator_ChangeDamageType : public UCardAugmentTextGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<int32, FText> DamageTypeAliases;

	FText GenerateText(const FCardAugmentData& AugmentData) const override;
};

UCLASS(Blueprintable)
class UCardAugmentShardNameGenerator_ChangeDamageType : public UCardAugmentShardTextGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<int32, FText> DamageTypeAliases;

	FText Generate(const FCardAugmentShard& ShardData) override;
};

UCLASS(Blueprintable)
class UCardAugmentShardDescGenerator_ChangeDamageType : public UCardAugmentShardTextGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<int32, FText> DamageTypeNames;

	FText Generate(const FCardAugmentShard& ShardData) override;
};


//Uses the Parameter.Magnitude to match damage types and change them to Parameter.VariantOption
UCLASS()
class UCardAugment_ChangeDamageType : public UCardAugment
{
	GENERATED_BODY()

	UCardAugment_ChangeDamageType();

public:
	void ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const override;
};