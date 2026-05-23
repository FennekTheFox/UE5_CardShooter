#pragma once

#include "GameplayTagContainer.h"
#include "Cards/CardIdentity.h"

#include "CardAugmentShard.generated.h"

class UCardAugment;

USTRUCT(BlueprintType)
struct FCardAugmentShard
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSoftClassPtr<UCardAugment> AugmentType;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTag ShardProperty;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float ShardMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 ShardOption = 0;

	bool operator==(const FCardAugmentShard& Other) const {
		return AugmentType == Other.AugmentType
			&& ShardProperty == Other.ShardProperty
			&& ShardMagnitude == Other.ShardMagnitude
			&& ShardOption == Other.ShardOption;
	}
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

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UDataTable> LookupTable;
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

USTRUCT(BlueprintType)
struct FParameterAugmentGeneratorRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FWeighteParameterValue> AddMagnitudes;
	UPROPERTY(BlueprintReadWrite)
	TArray<FWeighteParameterValue> MultiplyMagnitudes;
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