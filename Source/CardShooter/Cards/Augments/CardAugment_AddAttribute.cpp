#include "CardAugment_AddAttribute.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Card_Augment_AddAttribute, "Gameplay.Core.Cards.Augments.AddAttribute");

#define LOCTEXT_NAMESPACE "CardAugments"

UCardAugment_AddAttribute::UCardAugment_AddAttribute()
{
	static ConstructorHelpers::FClassFinder<UCardAugmentShardTextGenerator> ShardNameGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_ShardNameGen_AddAttribute"));
	static ConstructorHelpers::FClassFinder<UCardAugmentShardTextGenerator> ShardDescGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_ShardDescGen_AddAttribute"));
	static ConstructorHelpers::FClassFinder<UCardAugmentTextGenerator> AugNameGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_CardAugmentNameGenerator_AddAttribute"));
	static ConstructorHelpers::FClassFinder<UCardAugmentTextGenerator> AugDescGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_CardAugmentDescriptionGenerator_AddAttribute"));

	static ConstructorHelpers::FClassFinder<UCardAugmentBrushGenerator> BrushGenFinder(
		TEXT("/Game/Blueprint/Misc/BrushGenerators/BP_CardAugment_BrushGenerator_AddAttribute"));
	static ConstructorHelpers::FClassFinder<UCardAugmentShardBrushGenerator> ShardBrushGenFinder(
		TEXT("/Game/Blueprint/Misc/BrushGenerators/BP_CardAugmentShard_BrushGenerator_AddAttribute"));

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

	AugmentTag = TAG_Card_Augment_AddAttribute;
}

void UCardAugment_AddAttribute::ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const
{
	//for (const FCardAugmentParameter& Parameter : AugmentData.AugmentParameters)
	//{
	//	for (FSetByCallerMetaData& DataRef : DataToModify.SetByCallerMetaDataValues)
	//	{
	//		if (DataRef.DisplayType != ESetByCallerMagnitudeDisplayType::DamageType)
	//		{
	//			continue;
	//		}

	//		float& MagnitudeRef = DataRef.Magnitude;

	//		if (FMath::IsNearlyEqual(MagnitudeRef, Parameter.Magnitude, 0.05f))
	//		{
	//			MagnitudeRef = Parameter.VariantOption;
	//		}
	//	}
	//}
}

FText UCardAugmentNameGenerator_AddAttribute::GenerateText(const FCardAugmentData& AugmentData) const
{
	FText FormatingBase = LOCTEXT("Augment_ModParam_FormatingBase", "{0} Augment");

	FText AttributeName = FText::GetEmpty();

	for (auto&& KVPair : AttributeAliases)
	{
		if (KVPair.Key == AugmentData.AugmentParameters[0].ParameterTag)
		{
			AttributeName = KVPair.Value;
			break;
		}
	}
	return FText::Format(FormatingBase, AttributeName);
}


FText UCardAugmentDescriptionGenerator_AddAttribute::GenerateText(const FCardAugmentData& AugmentData) const
{
	FText FormatingBase = LOCTEXT("AugmentShardDescFormat", "Adds the \"{0}\" attribute.");

	FText AttributeText = FText::GetEmpty();

	for (auto&& KVPair : AttributeNames)
	{
		if (KVPair.Key == AugmentData.AugmentParameters[0].ParameterTag)
		{
			AttributeText = KVPair.Value;
			break;
		}
	}

	return FText::Format(FormatingBase, AttributeText);
}

FText UCardAugmentShardNameGenerator_AddAttribute::Generate(const FCardAugmentShard& ShardData)
{
	FText FormatingBase = LOCTEXT("Augment_ModParam_FormatingBase", "{0} Augment Shard, Ver. {1}");

	FText TypeText = FText::GetEmpty();

	for (auto&& KVPair : AttributeAliases)
	{
		if (KVPair.Key == ShardData.ShardProperty)
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

FText UCardAugmentShardDescGenerator_AddAttribute::Generate(const FCardAugmentShard& ShardData)
{
	FText FormatingBase = LOCTEXT("AugmentShardDescFormat", "Contains partial code for the \"{0}\" attribute.");


	FText TypeText = FText::GetEmpty();

	for (auto&& KVPair : AttributeNames)
	{
		if (KVPair.Key == ShardData.ShardProperty)
		{
			TypeText = KVPair.Value;
			break;
		}
	}

	return FText::Format(FormatingBase,	TypeText);
}

#undef LOCTEXT_NAMESPACE