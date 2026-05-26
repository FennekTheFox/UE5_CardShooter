#include "CardAugmentShard.h"
#include "CardAugment_ParameterModifier.h"
#include "Util/CardShooterUtility.h"

void UCardAugmentShardLibrary::GenerateShardsFromCards(const TArray<FCardIdentity>& InCards, TArray<FCardAugmentShard>& Outshards, const TArray<TSubclassOf<UCardAugmentShardGenerator>>& UsedGenerators)
{
	Outshards.Empty();

	for (const TSubclassOf<UCardAugmentShardGenerator>& GeneratorClass : UsedGenerators)
	{
		UCardAugmentShardGenerator* Generator = Cast<UCardAugmentShardGenerator>(GeneratorClass->GetDefaultObject());

		if (ensure(Generator))
		{
			for (const FCardIdentity& Card : InCards)
			{
				Generator->Generate(Card, Outshards);
			}
		}
	}
}

void UCardAugmentShardGenerator::Generate_Implementation(const FCardIdentity& Card, TArray<FCardAugmentShard>& OutShards) const
{
	return;
}


bool UItem_Stackable_CardAugmentShard::SameItem_Implementation(const UItem* Other) const
{
	if (const UItem_Stackable_CardAugmentShard* OtherAsShardItem = Cast<UItem_Stackable_CardAugmentShard>(Other))
	{
		return ShardData == OtherAsShardItem->ShardData;
	}

	return false;
}
	
void UItem_Stackable_CardAugmentShard::GetItemBrush_Implementation(FSlateBrush& OutBrush)
{
	UCardAugment::GetShardStyleData(ShardData, OutBrush);
}

FText UItem_Stackable_CardAugmentShard::GetItemName_Implementation()
{
	return UCardAugment::GetShardName(ShardData);
}

FText UItem_Stackable_CardAugmentShard::GetItemDescription_Implementation()
{
	return UCardAugment::GetShardDescription(ShardData);
}

bool UItem_Stackable_CardAugmentShard::SortAmongSames(const UItem* Other) const
{
	const UItem_Stackable_CardAugmentShard* OtherAsShard = Cast < UItem_Stackable_CardAugmentShard>(Other);

	if (ensure(OtherAsShard))
	{
		if (ShardData != OtherAsShard->ShardData)
		{
			return ShardData < OtherAsShard->ShardData;
		}

		return Super::SortAmongSames(Other);
	}

	return false;
}
