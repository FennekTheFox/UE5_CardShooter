#include "CardAugment_ModifySetByCallerParameters.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Card_Augment, "Gameplay.Core.Cards.Augments.ModifyParameter");

#define LOCTEXT_NAMESPACE "CardAugments"

UCardAugment_ModifySetByCallerParameter::UCardAugment_ModifySetByCallerParameter()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> AugmentChipIconFinder(
		TEXT("/Game/Assets/Textures/Icons/T_Icon_AugmentChip_Default"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> Shard1IconFinder(
		TEXT("/Game/Assets/Textures/Icons/T_Icon_AugmentShard_Alpha"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> Shard2IconFinder(
		TEXT("/Game/Assets/Textures/Icons/T_Icon_AugmentShard_Beta"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> Shard3IconFinder(
		TEXT("/Game/Assets/Textures/Icons/T_Icon_AugmentShard_Gamma"));
	static ConstructorHelpers::FClassFinder<UCardAugmentShardTextGenerator> ShardNameGen(
		TEXT("/Game/Blueprint/Misc/NameGenerators/BP_ShardNameGen_ParameterModifier"));
	static ConstructorHelpers::FClassFinder<UCardAugmentShardTextGenerator> ShardDescGen(
		TEXT("/Game/Blueprint/Misc/NameGenerators/BP_ShardDescGen_ParameterModifier"));

	if (ensure(ShardNameGen.Succeeded()))
	{
		ShardNameGenerator = Cast<UCardAugmentShardTextGenerator>(ShardNameGen.Class->GetDefaultObject());
	}
	if (ensure(ShardDescGen.Succeeded()))
	{
		ShardDescriptionGenerator = Cast<UCardAugmentShardTextGenerator>(ShardDescGen.Class->GetDefaultObject());
	}
	if (ensure(AugmentChipIconFinder.Succeeded()))
	{
		DefaultAugmentTexutre = AugmentChipIconFinder.Object;
	}	
	if (ensure(Shard1IconFinder.Succeeded()))
	{
		DefaultShardTextures.Add(ECardShardPiece::Piece1, Shard1IconFinder.Object);
	}	
	if (ensure(Shard2IconFinder.Succeeded()))
	{
		DefaultShardTextures.Add(ECardShardPiece::Piece2, Shard2IconFinder.Object);
	}	
	if (ensure(Shard3IconFinder.Succeeded()))
	{
		DefaultShardTextures.Add(ECardShardPiece::Piece3, Shard3IconFinder.Object);
	}

	AugmentTag = TAG_Card_Augment;
}

void UCardAugment_ModifySetByCallerParameter::ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const
{
	for (auto&& KVPair : AugmentData.AugmentParameters)
	{
		if (!ensure(KVPair.Value.VariantOption >= 0 && KVPair.Value.VariantOption <= 1))
		{
			continue;
		}


		for (FSetByCallerMetaData& DataRef : DataToModify.SetByCallerMetaDataValues)
		{
			if (DataRef.ScalingGroup != KVPair.Key)
			{
				continue;
			}

			float& MagnitudePtr = DataRef.Magnitude;

			switch ((EModifyOperationType)KVPair.Value.VariantOption)
			{
			case EModifyOperationType::Add: MagnitudePtr += KVPair.Value.Magnitude; break;
			case EModifyOperationType::Multiply: MagnitudePtr *= KVPair.Value.Magnitude; break;
			}
		}
	}
}

FText UCardAugmentNameGenerator_ModifyParameter::GenerateName(const FCardAugmentData& ShardData)
{
	return FText::FromString(TEXT("TODO: Generate Name for Modify Parameter augments"));
}

FText UCardAugmentShardNameGenerator_ModifyParameter::Generate(const FCardAugmentShard& ShardData)
{
	FText FormatingBase = LOCTEXT("ParamShardFormat", "{0} {1} Shard of {2}, Ver. {3}");

	FText MagnitudePrefix = LOCTEXT("ParamShard_MissingMagnPrefix", "MISSING MAG PREFIX");
	for (auto&& KVPair : PrefixMapping)
	{
		if (FMath::IsNearlyEqual(ShardData.ShardMagnitude, KVPair.Key, 0.025f))
		{
			MagnitudePrefix = KVPair.Value;
			break;
		}
	}

	FText TypePrefix = (ShardData.ShardOption == 0 ?
		LOCTEXT("ParamShard_AddiditePrefix", "Flux") :
		LOCTEXT("ParamShard_MultiplicativePrefix", "Cascade")
		);

	FText ParamSuffix = LOCTEXT("ParamShard_MissingParamSuffix", "MISSING PARAM SUFFIX");
	FText* FoundParamText = SuffixMapping.Find(ShardData.ShardProperty);
	if (FoundParamText)
	{
		ParamSuffix = *FoundParamText;
	}

	FText VersionText;
	switch (ShardData.PieceType)
	{
	case ECardShardPiece::Piece1: VersionText = LOCTEXT("AugShard_VersionAlpha", "Alpha"); break;
	case ECardShardPiece::Piece2: VersionText = LOCTEXT("AugShard_VersionBeta", "Beta"); break;
	case ECardShardPiece::Piece3: VersionText = LOCTEXT("AugShard_VersionGamma", "Gamma"); break;
	}

	return FText::Format(FormatingBase,
		MagnitudePrefix,
		TypePrefix,
		ParamSuffix,
		VersionText);
}

FText UCardAugmentShardDescGenerator_ModifyParameter::Generate(const FCardAugmentShard& ShardData)
{
	FText FormatingBase = LOCTEXT("AugmentShardDescFormat", "Type: {0} - Quality: {1}%");

	FText ScalingGroupText = LOCTEXT("AugShard_MissingScalingGroup", "MISSING SCALING GROUP");
	if (FText* SCTPtr = ScalingGroupNames.Find(ShardData.ShardProperty))
	{
		ScalingGroupText = *SCTPtr;
	}

	return FText::Format(FormatingBase,
		ScalingGroupText,
		FText::AsNumber((ShardData.ShardMagnitude - FMath::Sign(ShardData.ShardMagnitude)) * 100.f));
}

#undef LOCTEXT_NAMESPACE
