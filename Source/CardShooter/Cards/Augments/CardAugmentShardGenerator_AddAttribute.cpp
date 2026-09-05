#include "CardAugmentShardGenerator_AddAttribute.h"
#include "Util/CardShooterUtility.h"
#include "CardAugment_AddAttribute.h"

void UCardAugmentShardGenerator_AddAttribute::Generate_Implementation(const FCardIdentity& Card, TArray<FCardAugmentShard>& OutShards) const
{

	TArray<UCardEffectFragment_Attribute*> CardAttributes;
	UCardData* CardData = UCardShooterUtilityLibrary::GetCardDataFromIdentity(Card);
	CardData->GetCardAttributes(Card, CardAttributes);

	for (UCardEffectFragment_Attribute* Attribute : CardAttributes)
	{
		if (!Attribute->GetCanBeExtracted())
		{
			continue;
		}

		float RandomValue = FMath::FRand();

		if (RandomValue > Chance)
		{
			return;
		}

		FCardAugmentShard NewShard;
		NewShard.AugmentType = UCardAugment_AddAttribute::StaticClass();
		NewShard.PieceType = (EAugmentShardPiece)FMath::RandRange(0, 2);
		NewShard.ShardProperty = Attribute->AttributeTag;
		OutShards.Add(NewShard);
	}
}