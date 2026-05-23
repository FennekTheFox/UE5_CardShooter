#include "CardAugmentShard.h"
#include "CardAugment_ModifySetByCallerParameters.h"
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

void UCardAugmentShardGenerator_ParameterModifier::Generate_Implementation(const FCardIdentity& Card, TArray<FCardAugmentShard>& OutShards) const
{
	TArray<FGameplayTag> ParameterTags;
	UCardData* CardData = UCardShooterUtilityLibrary::GetCardDataFromIdentity(Card);
	CardData->GetAllUsedParameters(ParameterTags);

	if (ParameterTags.IsEmpty())
	{
		return;
	}

	int32 RandomIndex = FMath::RandRange(0, ParameterTags.Num()-1);
	FGameplayTag ChosenParameter = ParameterTags[RandomIndex];

	FString Discard;
	FString ParameterKey;
	ChosenParameter.ToString().Split(TEXT("."), &Discard, &ParameterKey, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	if (!ensure(LookupTable))
	{
		return;
	}

	FParameterAugmentGeneratorRow* GeneratorData = LookupTable->FindRow<FParameterAugmentGeneratorRow>(FName(*ParameterKey), TEXT("Parameter Lookup)"));

	if (ensure(GeneratorData))
	{
		float SumWeights = 0.f;

		for (const FWeighteParameterValue& Entry : GeneratorData->AddMagnitudes)
		{
			SumWeights += Entry.Weight;
		}
		for (const FWeighteParameterValue& Entry : GeneratorData->MultiplyMagnitudes)
		{
			SumWeights += Entry.Weight;
		}

		float RandomValue = FMath::RandRange(0.f, SumWeights);

		for (const FWeighteParameterValue& Entry : GeneratorData->AddMagnitudes)
		{
			RandomValue -= Entry.Weight;

			if (RandomValue <= 0.f)
			{
				FCardAugmentShard NewShard;
				NewShard.AugmentType = UCardAugment_ModifySetByCallerParameter::StaticClass();
				NewShard.ShardProperty = ChosenParameter;
				NewShard.ShardMagnitude = Entry.Magnitude;
				NewShard.ShardOption = 0; // for add
				OutShards.Add(NewShard);
			}
		}

		for (const FWeighteParameterValue& Entry : GeneratorData->MultiplyMagnitudes)
		{
			RandomValue -= Entry.Weight;

			if (RandomValue <= 0.f)
			{
				FCardAugmentShard NewShard;
				NewShard.AugmentType = UCardAugment_ModifySetByCallerParameter::StaticClass();
				NewShard.ShardProperty = ChosenParameter;
				NewShard.ShardMagnitude = Entry.Magnitude;
				NewShard.ShardOption = 1; // for multiply
				OutShards.Add(NewShard);
			}
		}
	}
}
