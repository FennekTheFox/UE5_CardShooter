#pragma once

#include "GameplayTagContainer.h"

#include "CardAugmentShard.generated.h"

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