#pragma once

#include "GameplayTagContainer.h"
#include "Cards/Fragments/CardExecution_AdditionalData.h"
#include "CardAugmentShard.h"
#include "Inventory/Item.h"

#include "CardAugment.generated.h"

#define MAX_AUGMENT_SOCKET_COUNT 3

UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardAugmentBrushGenerator : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
	void GenerateBrush(const FCardAugmentData& AugmentData, FSlateBrush& OutBrush) const;
};

UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardAugmentShardBrushGenerator : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
	void GenerateBrush(const FCardAugmentShard& AugmentData, FSlateBrush& OutBrush) const;
};

UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardAugmentTextGenerator : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FText GenerateText(const FCardAugmentData& AugmentData) const { return FText::GetEmpty(); };
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
	TMap<EAugmentShardPiece, class UTexture2D*> DefaultShardTextures;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor DefaultTint = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FLinearColor, FGameplayTagQuery> TextureTintMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCardAugmentBrushGenerator> AugmentBushGenerator = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCardAugmentShardBrushGenerator> AugmentShardBrushGenerator = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCardAugmentTextGenerator> AugmentNameGenerator = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCardAugmentTextGenerator> AugmentDescriptionGenerator = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCardAugmentShardTextGenerator> ShardNameGenerator = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCardAugmentShardTextGenerator> ShardDescriptionGenerator = nullptr;

	UFUNCTION(BlueprintPure)
	static void GetAugmentStyleData(const FCardAugmentData& Data, FSlateBrush& OutBrush);

	UFUNCTION(BlueprintPure)
	static FText GetAugmentName(const FCardAugmentData& Data);

	UFUNCTION(BlueprintPure)
	static FText GetAugmentDescription(const FCardAugmentData& Data);

	UFUNCTION(BlueprintPure)
	static void GetShardStyleData(const FCardAugmentShard& Data, FSlateBrush& OutBrush);

	UFUNCTION(BlueprintPure)
	static FText GetShardName(const FCardAugmentShard& Data);

	UFUNCTION(BlueprintPure)
	static FText GetShardDescription(const FCardAugmentShard& Data);
};

UCLASS(BlueprintType)
class UItem_CardAugment : public UItem 
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, SaveGame)
	FCardAugmentData AugmentData;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, SaveGame)
	FCardIdentityHandle UsedByCard;

	void GetItemBrush_Implementation(FSlateBrush& OutBrush) override;
	bool SameItem_Implementation(const UItem* Other) const override;
	FText GetItemName_Implementation() override;
	FText GetItemDescription_Implementation() override;


	bool SortAmongSames(const UItem* Other) const override;

};