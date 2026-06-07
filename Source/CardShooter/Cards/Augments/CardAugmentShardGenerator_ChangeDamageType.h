#pragma once

#include "CardAugmentShard.h"

#include "CardAugmentShardGenerator_ChangeDamageType.generated.h"

UCLASS(BlueprintType, Abstract, Blueprintable)
class UCardAugmentShardGenerator_ChangeDamageType : public UCardAugmentShardGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float Chance = 0.2f;

	void Generate_Implementation(const FCardIdentity& Card, TArray<FCardAugmentShard>& OutShards) const override;
};