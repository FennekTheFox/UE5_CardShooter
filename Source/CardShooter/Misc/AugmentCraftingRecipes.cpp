#include "AugmentCraftingRecipes.h"

#include "Cards/Augments/CardAugmentShard.h"
#include "Cards/Augments/CardAugment_ParameterModifier.h"

bool UAugmentRecipe_ParameterModifier::CanCraft_Implementation(const TArray<UItem*>& Materials) const
{
	if (!ensure(Materials.Num() == 3))
	{
		return false;
	}

	FGameplayTag ScalingGroup;
	//int32 ShardOption = -1;

	TArray<EAugmentShardPiece> Pieces{ EAugmentShardPiece::Piece1, EAugmentShardPiece::Piece2, EAugmentShardPiece::Piece3 };

	for (UItem* Material : Materials)
	{
		if (UItem_Stackable_CardAugmentShard* AsShard = Cast<UItem_Stackable_CardAugmentShard>(Material))
		{
			if (ScalingGroup.IsValid())
			{
				if (!(AsShard->ShardData.ShardProperty == ScalingGroup))
				{
					return false;
				}
			}
			else
			{
				ScalingGroup = AsShard->ShardData.ShardProperty;
			}

			//if (ShardOption == -1)
			//{
			//	ShardOption = AsShard->ShardData.ShardOption;
			//}
			//else
			//{
			//	if (AsShard->ShardData.ShardOption != ShardOption)
			//	{
			//		return false;
			//	}
			//}

			if (!(Pieces.Remove(AsShard->ShardData.PieceType) > 0))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

UItem* UAugmentRecipe_ParameterModifier::Craft_Implementation(const TArray<UItem*>& Materials, UObject* Outer)
{
	if (!ensure(CanCraft(Materials)))
	{
		return nullptr;
	}

	UItem_Stackable_CardAugmentShard* Neutral = Cast<UItem_Stackable_CardAugmentShard>(Materials[0]);
	UItem_Stackable_CardAugmentShard* Positive = Cast<UItem_Stackable_CardAugmentShard>(Materials[1]);
	UItem_Stackable_CardAugmentShard* Negative = Cast<UItem_Stackable_CardAugmentShard>(Materials[2]);

	FCardAugmentData NewAugment;
	NewAugment.AugmentClass = UCardAugment_ParameterModifier::StaticClass();
	NewAugment.AugmentTag = Cast<UCardAugment_ParameterModifier>(UCardAugment_ParameterModifier::StaticClass()->GetDefaultObject())->AugmentTag;

	int32 Option = Neutral->ShardData.ShardOption;

	float NeutralScore = 1.0f - FMath::Clamp(
		(FMath::Abs(Neutral->ShardData.ShardMagnitude) - 1.0f) / 0.5f,
		0.0f,
		1.0f
	);

	float PositiveScore = FMath::Clamp(
		(Positive->ShardData.ShardMagnitude + 1.5f) / 3.0f,
		0.0f,
		1.0f
	);

	float NegativeScore = FMath::Clamp(
		(1.5f - Negative->ShardData.ShardMagnitude) / 3.0f,
		0.0f,
		1.0f
	);

	float RawAlpha = (NeutralScore + PositiveScore + NegativeScore) / 3.0f;
	float MappedAlpha = DistributionCurve->GetFloatValue(RawAlpha);

	FCardAugmentParameter NewParameter;
	NewParameter.Magnitude = MappedAlpha;
	NewParameter.VariantOption = Option;

	NewAugment.AugmentParameters.Add(Neutral->ShardData.ShardProperty, NewParameter);

	UItem_CardAugment* NewAugmentItem = NewObject<UItem_CardAugment>(Outer);
	NewAugmentItem->AugmentData = NewAugment;
	return NewAugmentItem;
}

