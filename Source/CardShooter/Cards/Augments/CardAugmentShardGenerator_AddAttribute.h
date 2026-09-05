#pragma once

#include "CardAugmentShard.h"

#include "CardAugmentShardGenerator_AddAttribute.generated.h"

UCLASS(BlueprintType, Abstract, Blueprintable)
class UCardAugmentShardGenerator_AddAttribute : public UCardAugmentShardGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float Chance = 0.5f;

	void Generate_Implementation(const FCardIdentity& Card, TArray<FCardAugmentShard>& OutShards) const override;
};