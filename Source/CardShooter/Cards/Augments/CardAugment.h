#pragma once

#include "GameplayTagContainer.h"
#include "Cards/Fragments/CardExecution_AdditionalData.h"
#include "CardAugmentShard.h"

#include "CardAugment.generated.h"

#define MAX_AUGMENT_SOCKET_COUNT 3

UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardAugmentNameGenerator : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FText GenerateName(const FCardAugmentData& AugmentData) { return FText::GetEmpty(); };
};

UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardAugmentShardTextGenerator : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FText Generate(const FCardAugmentShard& ShardData) { return FText::GetEmpty(); };
};

UCLASS(BlueprintType, Abstract)
class UCardAugment : public UObject
{
	GENERATED_BODY()

public:
	virtual void ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AugmentTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* DefaultAugmentTexutre;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<ECardShardPiece, class UTexture2D*> DefaultShardTextures;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor DefaultTint = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FLinearColor, FGameplayTagQuery> TextureTintMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCardAugmentNameGenerator> AugmentNameGenerator = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCardAugmentShardTextGenerator> ShardNameGenerator = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCardAugmentShardTextGenerator> ShardDescriptionGenerator = nullptr;

	UFUNCTION(BlueprintPure)
	static void GetAugmentStyleData(const FCardAugmentData& Data, class UTexture2D*& Texture, FLinearColor& Tint);

	UFUNCTION(BlueprintPure)
	static FText GetAugmentName(const FCardAugmentData& Data);

	UFUNCTION(BlueprintPure)
	static void GetShardStyleData(const FCardAugmentShard& Data, class UTexture2D*& Texture, FLinearColor& Tint);

	UFUNCTION(BlueprintPure)
	static FText GetShardName(const FCardAugmentShard& Data);

	UFUNCTION(BlueprintPure)
	static FText GetShardDescription(const FCardAugmentShard& Data);
};