#include "CardAugment_ChangeDamageType.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Card_Augment_ChangeDamageType, "Gameplay.Core.Cards.Augments.ChangeDamageType");

#define LOCTEXT_NAMESPACE "CardAugments"

UCardAugment_ChangeDamageType::UCardAugment_ChangeDamageType()
{
	static ConstructorHelpers::FClassFinder<UCardAugmentShardTextGenerator> ShardNameGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_ShardNameGen_ChangeDamageType"));
	static ConstructorHelpers::FClassFinder<UCardAugmentShardTextGenerator> ShardDescGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_ShardDescGen_ChangeDamageType"));
	static ConstructorHelpers::FClassFinder<UCardAugmentTextGenerator> AugNameGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_CardAugmentNameGenerator_ChangeDamageType"));
	static ConstructorHelpers::FClassFinder<UCardAugmentTextGenerator> AugDescGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_CardAugmentDescriptionGenerator_ChangeDamageType"));

	static ConstructorHelpers::FClassFinder<UCardAugmentBrushGenerator> BrushGenFinder(
		TEXT("/Game/Blueprint/Misc/BrushGenerators/BP_CardAugment_BrushGenerator_ChangeDamageType"));
	static ConstructorHelpers::FClassFinder<UCardAugmentShardBrushGenerator> ShardBrushGenFinder(
		TEXT("/Game/Blueprint/Misc/BrushGenerators/BP_CardAugmentShard_BrushGenerator_ChangeDamageType"));

	if (ensure(BrushGenFinder.Succeeded()))
	{
		AugmentBushGenerator = Cast<UCardAugmentBrushGenerator>(BrushGenFinder.Class->GetDefaultObject());
	}

	if (ensure(ShardBrushGenFinder.Succeeded()))
	{
		AugmentShardBrushGenerator = Cast<UCardAugmentShardBrushGenerator>(ShardBrushGenFinder.Class->GetDefaultObject());
	}

	if (ensure(AugNameGenFinder.Succeeded()))
	{
		AugmentNameGenerator = Cast<UCardAugmentTextGenerator>(AugNameGenFinder.Class->GetDefaultObject());
	}
	if (ensure(AugDescGenFinder.Succeeded()))
	{
		AugmentDescriptionGenerator = Cast<UCardAugmentTextGenerator>(AugDescGenFinder.Class->GetDefaultObject());
	}
	if (ensure(ShardNameGenFinder.Succeeded()))
	{
		ShardNameGenerator = Cast<UCardAugmentShardTextGenerator>(ShardNameGenFinder.Class->GetDefaultObject());
	}
	if (ensure(ShardDescGenFinder.Succeeded()))
	{
		ShardDescriptionGenerator = Cast<UCardAugmentShardTextGenerator>(ShardDescGenFinder.Class->GetDefaultObject());
	}

	AugmentTag = TAG_Card_Augment_ChangeDamageType;
}

void UCardAugment_ChangeDamageType::ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const
{
	for (const FCardAugmentParameter& Parameter : AugmentData.AugmentParameters)
	{
		for (FSetByCallerMetaData& DataRef : DataToModify.SetByCallerMetaDataValues)
		{
			if (DataRef.DisplayType != ESetByCallerMagnitudeDisplayType::DamageType)
			{
				continue;
			}

			float& MagnitudeRef = DataRef.Magnitude;

			if (FMath::IsNearlyEqual(MagnitudeRef, Parameter.Magnitude, 0.05f))
			{
				MagnitudeRef = Parameter.VariantOption;
			}
		}
	}
}

FText UCardAugmentNameGenerator_ChangeDamageType::GenerateText(const FCardAugmentData& AugmentData) const
{
	FText FormatingBase = LOCTEXT("Augment_ModParam_FormatingBase", "{0}-Morphing {1} Augment");

	if (!ensure(AugmentData.AugmentParameters.Num() == 1))
	{
		return FText::FromString(TEXT("TODO: Generate Name for Modify Parameter augments with more than 1 parameter"));
	}

	FText FromTypeText = FText::GetEmpty();

	for (auto&& KVPair : DamageTypeAliases)
	{
		if (FMath::IsNearlyEqual(KVPair.Key, AugmentData.AugmentParameters[0].Magnitude, 0.05f))
		{
			FromTypeText = KVPair.Value;
			break;
		}
	}

	FText ToTypeText = FText::GetEmpty();

	for (auto&& KVPair : DamageTypeAliases)
	{
		if (FMath::IsNearlyEqual(KVPair.Key, AugmentData.AugmentParameters[0].VariantOption, 0.05f))
		{
			ToTypeText = KVPair.Value;
			break;
		}
	}

	return FText::Format(FormatingBase, ToTypeText, FromTypeText);
}


FText UCardAugmentDescriptionGenerator_ChangeDamageType::GenerateText(const FCardAugmentData& AugmentData) const
{
	FText FormatingBase = LOCTEXT("AugmentShardDescFormat", "Changes {0} to {1}");

	if (!ensure(AugmentData.AugmentParameters.Num() == 1))
	{
		return FText::FromString(TEXT("TODO: Generate Name for Modify Parameter augments with more than 1 parameter"));
	}

	FText FromTypeText = FText::GetEmpty();

	for (auto&& KVPair : DamageTypeNames)
	{
		if (FMath::IsNearlyEqual(KVPair.Key, AugmentData.AugmentParameters[0].Magnitude, 0.05f))
		{
			FromTypeText = KVPair.Value;
			break;
		}
	}

	FText ToTypeText = FText::GetEmpty();

	for (auto&& KVPair : DamageTypeNames)
	{
		if (FMath::IsNearlyEqual(KVPair.Key, AugmentData.AugmentParameters[0].VariantOption, 0.05f))
		{
			ToTypeText = KVPair.Value;
			break;
		}
	}

	return FText::Format(FormatingBase, FromTypeText, ToTypeText);
}

FText UCardAugmentShardNameGenerator_ChangeDamageType::Generate(const FCardAugmentShard& ShardData)
{
	FText FormatingBase = LOCTEXT("Augment_ModParam_FormatingBase", "{0}-Morphing Augment Shard, Ver. {1}");

	FText TypeText = FText::GetEmpty();

	for (auto&& KVPair : DamageTypeAliases)
	{
		if (FMath::IsNearlyEqual(KVPair.Key, ShardData.ShardOption, 0.05f))
		{
			TypeText = KVPair.Value;
			break;
		}
	}

	FText VersionText;
	switch (ShardData.PieceType)
	{
	case EAugmentShardPiece::Piece1: VersionText = LOCTEXT("AugShard_VersionAlpha", "Alpha"); break;
	case EAugmentShardPiece::Piece2: VersionText = LOCTEXT("AugShard_VersionBeta", "Beta"); break;
	case EAugmentShardPiece::Piece3: VersionText = LOCTEXT("AugShard_VersionGamma", "Gamma"); break;
	}

	return FText::Format(FormatingBase,
		TypeText,
		VersionText);
}

FText UCardAugmentShardDescGenerator_ChangeDamageType::Generate(const FCardAugmentShard& ShardData)
{
	FText FormatingBase = LOCTEXT("AugmentShardDescFormat", "Resonates with {0}");


	FText TypeText = FText::GetEmpty();

	for (auto&& KVPair : DamageTypeNames)
	{
		if (FMath::IsNearlyEqual(KVPair.Key, ShardData.ShardOption, 0.05f))
		{
			TypeText = KVPair.Value;
			break;
		}
	}

	return FText::Format(FormatingBase,	TypeText);
}

#undef LOCTEXT_NAMESPACE