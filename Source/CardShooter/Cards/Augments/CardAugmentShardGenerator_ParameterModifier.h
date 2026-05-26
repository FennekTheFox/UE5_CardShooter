#pragma once

#include "CardAugmentShard.h"

#include "CardAugmentShardGenerator_ParameterModifier.generated.h"

UCLASS(BlueprintType, Abstract, Blueprintable)
class UCardAugmentShardGenerator_ParameterModifier : public UCardAugmentShardGenerator
{
	GENERATED_BODY()

public:

	void Generate_Implementation(const FCardIdentity& Card, TArray<FCardAugmentShard>& OutShards) const override;
};