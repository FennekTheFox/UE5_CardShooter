#include "AugmentCraftingRecipes.h"

#include "Cards/Augments/CardAugmentShard.h"
#include "Cards/Augments/CardAugment_ParameterModifier.h"
#include "Cards/Augments/CardAugment_ChangeDamageType.h"
#include "../Cards/Augments/CardAugment_AddAttribute.h"

bool UAugmentRecipe_ParameterModifier::CanCraft_Implementation(const TArray<UItem*>& Materials) const
{
	if (!ensure(Materials.Num() == 3))
	{
		return false;
	}


	for (UItem* Material : Materials)
	{
		if (auto* AsShard = Cast<UItem_Stackable_CardAugmentShard>(Material))
		{
			if (AsShard->ShardData.AugmentType != FSoftClassPath(UCardAugment_ParameterModifier::StaticClass()))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
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

	if (!(Neutral && Positive && Negative))
	{
		return nullptr;
	}

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
	NewParameter.ParameterTag = Neutral->ShardData.ShardProperty;
	NewParameter.Magnitude = MappedAlpha;
	NewParameter.VariantOption = Option;

	NewAugment.AugmentParameters.Add(NewParameter);
	
	UItem_CardAugment* NewAugmentItem = NewObject<UItem_CardAugment>(Outer);
	NewAugmentItem->AugmentData = NewAugment;
	return NewAugmentItem;
}


bool UAugmentRecipe_ChangeDamageType::CanCraft_Implementation(const TArray<UItem*>& Materials) const
{
	if (!ensure(Materials.Num() == 3))
	{
		return false;
	}

	UItem_Stackable_CardAugmentShard* Neutral = Cast<UItem_Stackable_CardAugmentShard>(Materials[0]);
	UItem_Stackable_CardAugmentShard* Positive = Cast<UItem_Stackable_CardAugmentShard>(Materials[1]);
	UItem_Stackable_CardAugmentShard* Negative = Cast<UItem_Stackable_CardAugmentShard>(Materials[2]);

	if (!(Neutral && Positive && Negative))
	{
		return false;
	}

	for (UItem* Material : Materials)
	{
		if (auto* AsShard = Cast<UItem_Stackable_CardAugmentShard>(Material))
		{
			if (AsShard->ShardData.AugmentType != FSoftClassPath(UCardAugment_ChangeDamageType::StaticClass()))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	//The from and to values of the damage type conversion need to differ
	if (Negative->ShardData.ShardMagnitude == Positive->ShardData.ShardMagnitude)
	{
		return false;
	}

	//The neutral needs to share in either damage type
	if (Negative->ShardData.ShardOption != Neutral->ShardData.ShardOption &&
		Positive->ShardData.ShardOption != Neutral->ShardData.ShardOption)
	{
		return false;
	}

	return true;
}

UItem* UAugmentRecipe_ChangeDamageType::Craft_Implementation(const TArray<UItem*>& Materials, UObject* Outer)
{
	if (!ensure(CanCraft(Materials)))
	{
		return nullptr;
	}

	UItem_Stackable_CardAugmentShard* Neutral = Cast<UItem_Stackable_CardAugmentShard>(Materials[0]);
	UItem_Stackable_CardAugmentShard* Positive = Cast<UItem_Stackable_CardAugmentShard>(Materials[1]);
	UItem_Stackable_CardAugmentShard* Negative = Cast<UItem_Stackable_CardAugmentShard>(Materials[2]);

	if (!(Neutral && Positive && Negative))
	{
		return nullptr;
	}

	FCardAugmentData NewAugment;
	NewAugment.AugmentClass = UCardAugment_ChangeDamageType::StaticClass();
	NewAugment.AugmentTag = Cast<UCardAugment_ChangeDamageType>(UCardAugment_ChangeDamageType::StaticClass()->GetDefaultObject())->AugmentTag;

	FCardAugmentParameter NewParameter;
	NewParameter.ParameterTag = Neutral->ShardData.ShardProperty;
	NewParameter.Magnitude = Negative->ShardData.ShardOption;
	NewParameter.VariantOption = Positive->ShardData.ShardOption;

	NewAugment.AugmentParameters.Add(NewParameter);

	UItem_CardAugment* NewAugmentItem = NewObject<UItem_CardAugment>(Outer);
	NewAugmentItem->AugmentData = NewAugment;
	return NewAugmentItem;
}


bool UAugmentRecipe_AddAttribute::CanCraft_Implementation(const TArray<UItem*>& Materials) const
{
	if (!ensure(Materials.Num() == 3))
	{
		return false;
	}

	//Make sure all 3 shards are of the same type
	for (UItem* Material : Materials)
	{
		if (auto* AsShard = Cast<UItem_Stackable_CardAugmentShard>(Material))
		{
			if (AsShard->ShardData.AugmentType != FSoftClassPath(UCardAugment_AddAttribute::StaticClass()))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	UItem_Stackable_CardAugmentShard* Neutral = Cast<UItem_Stackable_CardAugmentShard>(Materials[0]);
	UItem_Stackable_CardAugmentShard* Positive = Cast<UItem_Stackable_CardAugmentShard>(Materials[1]);
	UItem_Stackable_CardAugmentShard* Negative = Cast<UItem_Stackable_CardAugmentShard>(Materials[2]);

	if (!(Neutral && Positive && Negative))
	{
		return false;
	}

	//The neutral needs to share in either damage type
	if (Negative->ShardData.ShardProperty != Neutral->ShardData.ShardProperty ||
		Positive->ShardData.ShardProperty != Neutral->ShardData.ShardProperty)
	{
		return false;
	}

	return true;
}

UItem* UAugmentRecipe_AddAttribute::Craft_Implementation(const TArray<UItem*>& Materials, UObject* Outer)
{
	if (!ensure(CanCraft(Materials)))
	{
		return nullptr;
	}

	UItem_Stackable_CardAugmentShard* Neutral = Cast<UItem_Stackable_CardAugmentShard>(Materials[0]);
	UItem_Stackable_CardAugmentShard* Positive = Cast<UItem_Stackable_CardAugmentShard>(Materials[1]);
	UItem_Stackable_CardAugmentShard* Negative = Cast<UItem_Stackable_CardAugmentShard>(Materials[2]);

	if (!(Neutral && Positive && Negative))
	{
		return nullptr;
	}

	FCardAugmentData NewAugment;
	NewAugment.AugmentClass = UCardAugment_AddAttribute::StaticClass();
	NewAugment.AugmentTag = Cast<UCardAugment_AddAttribute>(UCardAugment_AddAttribute::StaticClass()->GetDefaultObject())->AugmentTag;

	FCardAugmentParameter NewParameter;
	NewParameter.ParameterTag = Neutral->ShardData.ShardProperty;
	NewParameter.Magnitude = 0;
	NewParameter.VariantOption = 0;

	NewAugment.AugmentParameters.Add(NewParameter);

	UItem_CardAugment* NewAugmentItem = NewObject<UItem_CardAugment>(Outer);
	NewAugmentItem->AugmentData = NewAugment;
	return NewAugmentItem;
}
