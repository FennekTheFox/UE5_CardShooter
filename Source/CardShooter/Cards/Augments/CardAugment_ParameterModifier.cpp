#include "CardAugment_ParameterModifier.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Card_Augment, "Gameplay.Core.Cards.Augments.ParameterModifier");

#define LOCTEXT_NAMESPACE "CardAugments"

UCardAugment_ParameterModifier::UCardAugment_ParameterModifier()
{
	static ConstructorHelpers::FClassFinder<UCardAugmentShardTextGenerator> ShardNameGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_ShardNameGen_ParameterModifier"));
	static ConstructorHelpers::FClassFinder<UCardAugmentShardTextGenerator> ShardDescGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_ShardDescGen_ParameterModifier"));
	static ConstructorHelpers::FClassFinder<UCardAugmentTextGenerator> AugNameGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_CardAugmentNameGenerator_ParameterModifier"));
	static ConstructorHelpers::FClassFinder<UCardAugmentTextGenerator> AugDescGenFinder(
		TEXT("/Game/Blueprint/Misc/TextGenerators/BP_CardAugmentDescriptionGenerator_ParameterModifier"));
	static ConstructorHelpers::FClassFinder<UMagnitudeCalculation_ParameterModifier> MagCalcFinder(
		TEXT("/Game/Blueprint/Misc/BP_AugmentMagnitudeCalculation_ParameterModifier"));

	static ConstructorHelpers::FClassFinder<UCardAugmentBrushGenerator> BrushGenFinder(
		TEXT("/Game/Blueprint/Misc/BrushGenerators/BP_CardAugment_BrushGenerator_ParameterModifier"));
	static ConstructorHelpers::FClassFinder<UCardAugmentShardBrushGenerator> ShardBrushGenFinder(
		TEXT("/Game/Blueprint/Misc/BrushGenerators/BP_CardAugmentShard_BrushGenerator_ParameterModifier"));

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
	if (ensure(MagCalcFinder.Succeeded()))
	{
		MagnitudeCalculation = Cast<UMagnitudeCalculation_ParameterModifier>(MagCalcFinder.Class->GetDefaultObject());
	}

	AugmentTag = TAG_Card_Augment;
}

void UCardAugment_ParameterModifier::ApplyAugment(FCardExecution_AdditionalData& DataToModify, const FCardAugmentData& AugmentData) const
{
	if (!ensure(MagnitudeCalculation))
	{
		return;
	}

	for (const FCardAugmentParameter& Parameter : AugmentData.AugmentParameters)
	{
		if (!ensure(Parameter.VariantOption >= 0 && Parameter.VariantOption <= 1))
		{
			continue;
		}


		for (FSetByCallerMetaData& DataRef : DataToModify.SetByCallerMetaDataValues)
		{
			if (DataRef.ScalingGroup != Parameter.ParameterTag)
			{
				continue;
			}

			float& MagnitudePtr = DataRef.Magnitude;

			float AugmentMagnitude = 0.f;
			MagnitudeCalculation->CalculateMagnitude(Parameter, AugmentMagnitude);

			switch ((EModifyOperationType)Parameter.VariantOption)
			{
			case EModifyOperationType::Add: MagnitudePtr = FMath::Clamp(MagnitudePtr += AugmentMagnitude, DataRef.MagnitudeRange.GetLowerBound().GetValue(), DataRef.MagnitudeRange.GetUpperBound().GetValue()); break;
			case EModifyOperationType::Multiply:  MagnitudePtr = FMath::Clamp(MagnitudePtr *= AugmentMagnitude, DataRef.MagnitudeRange.GetLowerBound().GetValue(), DataRef.MagnitudeRange.GetUpperBound().GetValue()); break;
			}
		}
	}
}

FText UCardAugmentNameGenerator_ModifyParameter::GenerateText(const FCardAugmentData& AugmentData) const
{
	FText FormatingBase = LOCTEXT("Augment_ModParam_FormatingBase", "{0} {1} Augment of {2}");

	if (!ensure(AugmentData.AugmentParameters.Num() == 1))
	{
		return FText::FromString(TEXT("TODO: Generate Name for Modify Parameter augments with more than 1 parameter"));
	}

	for (const FCardAugmentParameter& Parameter : AugmentData.AugmentParameters)
	{
		const FText* SuffixPtr = SuffixMapping.Find(Parameter.ParameterTag);

		if (!ensure(SuffixPtr))
		{
			return LOCTEXT("Augment_ModParam_MisingSuffix", "MISSING PARAM SUFfIX");
		}

		FText Prefix = LOCTEXT("Augment_ModParam_MisingPrefix", "MISSING PARAM PREFIX");

		if (FMath::IsNearlyEqual(Parameter.Magnitude, 1.0f, 0.01f))
		{
			Prefix = LOCTEXT("Augment_ModParam_Perfect", "Perfect");
		}
		else if (FMath::IsNearlyEqual(Parameter.Magnitude, 0.0f, 0.01f))
		{
			Prefix = LOCTEXT("Augment_ModParam_PerfectInverse", "Perfect Inverse");
		}
		else
		{
			for (auto&& Entry : PrefixMapping)
			{
				if (FMath::IsNearlyEqual(Entry.Key, Parameter.Magnitude, 0.125))
				{
					Prefix = Entry.Value;
					break;
				}
			}
		}

		FText TypePrefix = (Parameter.VariantOption == 0 ?
			LOCTEXT("ParamShard_AddiditePrefix", "Flux") :
			LOCTEXT("ParamShard_MultiplicativePrefix", "Cascade")
			);
		return FText::Format(FormatingBase,
			Prefix,
			TypePrefix,
			*SuffixPtr);
	}


	return FText::FromString(TEXT("TODO: Generate Name for Modify Parameter augments"));
}


FText UCardAugmentDescriptionGenerator_ModifyParameter::GenerateText(const FCardAugmentData& AugmentData) const
{
	FText FormatingBase = LOCTEXT("AugmentShardDescFormat", "Type: {0} - Effect: {1}");

	for (const FCardAugmentParameter& Parameter : AugmentData.AugmentParameters)
	{
		const FText* TypeText = ScalingGroupNames.Find(Parameter.ParameterTag);

		if (!ensure(TypeText))
		{
			return FText::GetEmpty();
		}

		UCardAugment_ParameterModifier* AugmentCDO = Cast< UCardAugment_ParameterModifier>(UCardAugment_ParameterModifier::StaticClass()->GetDefaultObject());

		if (ensure(AugmentCDO))
		{
			FSetByCallerMetaData TempMetaData;
			FModifierRange* ModRange = nullptr;
			if (Parameter.VariantOption == 0) ModRange = AugmentCDO->MagnitudeCalculation->AdditiveRanges.Find(Parameter.ParameterTag);
			if (Parameter.VariantOption == 1) ModRange = AugmentCDO->MagnitudeCalculation->MultiplicativeRanges.Find(Parameter.ParameterTag);

			if (ensure(ModRange))
			{
				AugmentCDO->MagnitudeCalculation->CalculateMagnitude(Parameter, TempMetaData.Magnitude);
				TempMetaData.DisplayType = ModRange->DisplayType;

				FText EffectText = TempMetaData.GetDisplayText();

				return FText::Format(FormatingBase,
					*TypeText,
					EffectText);
			}
		}
	}

	return FText::FromString(TEXT("TODO: Generate Desc for Modify Parameter augments"));
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
	case EAugmentShardPiece::Piece1: VersionText = LOCTEXT("AugShard_VersionAlpha", "Alpha"); break;
	case EAugmentShardPiece::Piece2: VersionText = LOCTEXT("AugShard_VersionBeta", "Beta"); break;
	case EAugmentShardPiece::Piece3: VersionText = LOCTEXT("AugShard_VersionGamma", "Gamma"); break;
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

bool UMagnitudeCalculation_ParameterModifier::CalculateMagnitude(const FCardAugmentParameter& Parameter, float& OutMagnitude) const
{
	const TMap<FGameplayTag, FModifierRange>* RangeMap = nullptr;

	if (Parameter.VariantOption == 0)
	{
		RangeMap = &AdditiveRanges;
	}
	else
	{
		RangeMap = &MultiplicativeRanges;
	}

	if (!ensure(RangeMap))
	{
		return false;
	}

	const FModifierRange* ModRange = RangeMap->Find(Parameter.ParameterTag);

	if (!ensure(ModRange))
	{
		return false;
	}

	OutMagnitude = FMath::Lerp(ModRange->Range.GetLowerBoundValue(),
		ModRange->Range.GetUpperBoundValue(),
		Parameter.Magnitude);
	return true;
}

#undef LOCTEXT_NAMESPACE