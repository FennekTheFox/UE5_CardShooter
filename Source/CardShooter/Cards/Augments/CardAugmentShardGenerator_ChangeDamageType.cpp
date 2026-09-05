#include "CardAugmentShardGenerator_ChangeDamageType.h"
#include "Util/CardShooterUtility.h"
#include "CardAugment_ChangeDamageType.h"

void UCardAugmentShardGenerator_ChangeDamageType::Generate_Implementation(const FCardIdentity& Card, TArray<FCardAugmentShard>& OutShards) const
{
	float RandomValue = FMath::FRand();

	if (RandomValue > Chance)
	{
		return;
	}

	TArray<FGameplayTag> ParameterTags;
	UCardData* CardData = UCardShooterUtilityLibrary::GetCardDataFromIdentity(Card);

	TArray<UCardParameter*> Params;
	CardData->GetParametersByPredicate(Params, [](const UCardParameter* Param)
		{
			if ((Param->MetaData.DisplayType == ESetByCallerMagnitudeDisplayType::DamageType))
			{
				return true;
			}
			return false;
		});

	if (Params.IsEmpty())
	{
		return;
	}

	int32 RandomIndex = FMath::RandRange(0, Params.Num() - 1);

	FCardAugmentShard NewShard;
	NewShard.AugmentType = UCardAugment_ChangeDamageType::StaticClass();
	NewShard.PieceType = (EAugmentShardPiece)FMath::RandRange(0, 2);
	NewShard.ShardMagnitude = 0.f;
	NewShard.ShardOption = Params[RandomIndex]->MetaData.Magnitude;
	OutShards.Add(NewShard);
}