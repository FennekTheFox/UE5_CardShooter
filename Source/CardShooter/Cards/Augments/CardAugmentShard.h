#pragma once

#include "GameplayTagContainer.h"
#include "Cards/CardIdentity.h"
#include "Inventory/Item.h"

#include "CardAugmentShard.generated.h"

class UCardAugment;

UENUM(BlueprintType)
enum class ECardShardPiece : uint8
{
	Piece1 = 0,
	Piece2 = 1,
	Piece3 = 2
};

USTRUCT(BlueprintType)
struct FCardAugmentShard
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, SaveGame)
	FSoftClassPath AugmentType;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, SaveGame)
	FGameplayTag ShardProperty;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, SaveGame)
	ECardShardPiece PieceType = ECardShardPiece::Piece1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, SaveGame)
	float ShardMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, SaveGame)
	int32 ShardOption = 0;

	bool operator==(const FCardAugmentShard& Other) const {
		return AugmentType == Other.AugmentType
			&& ShardProperty == Other.ShardProperty
			&& ShardMagnitude == Other.ShardMagnitude
			&& ShardOption == Other.ShardOption;
	}
};

UCLASS(BlueprintType)
class UItem_Stackable_CardAugmentShard : public UItem_Stackable
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, SaveGame, meta = (ExposeOnSpawn = true))
	FCardAugmentShard ShardData;

	bool SameItem_Implementation(const UItem* Other) const override;
	void GetItemBrush_Implementation(FSlateBrush& OutBrush) override;
	FText GetItemName_Implementation() override;
	FText GetItemDescription_Implementation() override;

};

FORCEINLINE uint32 GetTypeHash(const FCardAugmentShard& Shard)
{
	uint32 Hash = GetTypeHash(Shard.AugmentType);
	Hash = HashCombine(Hash, GetTypeHash(Shard.ShardProperty));
	Hash = HashCombine(Hash, GetTypeHash(Shard.ShardMagnitude));
	Hash = HashCombine(Hash, GetTypeHash(Shard.ShardOption));
	return Hash;
}

UCLASS(BlueprintType, Abstract, Blueprintable)
class UCardAugmentShardGenerator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Generate(const FCardIdentity& Card, TArray<FCardAugmentShard>& OutShards) const;
};

USTRUCT(BlueprintType)
struct FWeighteParameterValue
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	float Weight = 1.f;

	UPROPERTY(BlueprintReadWrite)
	float  Magnitude = 0.f;
};
UCLASS(BlueprintType, Abstract, Blueprintable)
class UCardAugmentShardGenerator_ParameterModifier : public UCardAugmentShardGenerator
{
	GENERATED_BODY()

public:

	void Generate_Implementation(const FCardIdentity& Card, TArray<FCardAugmentShard>& OutShards) const override;
};

UCLASS()
class UCardAugmentShardLibrary : public UBlueprintFunctionLibrary
{

	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void GenerateShardsFromCards(const TArray<FCardIdentity>& InCards, TArray<FCardAugmentShard>& Outshards, const TArray<TSubclassOf<UCardAugmentShardGenerator>>& UsedGenerators);
};