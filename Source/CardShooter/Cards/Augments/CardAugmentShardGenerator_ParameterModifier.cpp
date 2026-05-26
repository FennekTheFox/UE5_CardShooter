#include "CardAugmentShardGenerator_ParameterModifier.h"
#include "Util/CardShooterUtility.h"
#include "CardAugment_ParameterModifier.h"

void UCardAugmentShardGenerator_ParameterModifier::Generate_Implementation(const FCardIdentity& Card, TArray<FCardAugmentShard>& OutShards) const
{
	TArray<FGameplayTag> ParameterTags;
	UCardData* CardData = UCardShooterUtilityLibrary::GetCardDataFromIdentity(Card);
	CardData->GetAllUsedScalingGroups(ParameterTags);

	if (ParameterTags.IsEmpty())
	{
		ensure(false);
		return;
	}

	int32 RandomIndex = FMath::RandRange(0, ParameterTags.Num() - 1);
	FGameplayTag ChosenParameter = ParameterTags[RandomIndex];

	FString Discard;
	FString ParameterKey;
	ChosenParameter.ToString().Split(TEXT("."), &Discard, &ParameterKey, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	float U1 = FMath::FRand();
	float U2 = FMath::FRand();
	float T = FMath::FRand();
	// Standard normal distribution (mean=0, stddev=1)
	float Z = FMath::Sqrt(-2.f * FMath::Loge(U1))
		* FMath::Cos(2.f * PI * U2);
	// Convert to [0,1] centered at 0.5
	// 3 sigma ≈ covers most values
	float BiasedT = FMath::Clamp(0.5f + Z / 6.f, 0.f, 1.f);
	float Magnitude = 1.0f + BiasedT * 0.5f;
	Magnitude = FMath::RoundToFloat(Magnitude / 0.05f) * 0.05f;
	Magnitude *= (FMath::FRand() > 0.5f ? 1.f : -1.f);

	FCardAugmentShard NewShard;
	NewShard.AugmentType = UCardAugment_ParameterModifier::StaticClass();
	NewShard.PieceType = (EAugmentShardPiece)FMath::RandRange(0, 2);
	NewShard.ShardProperty = ChosenParameter;
	NewShard.ShardMagnitude = Magnitude;
	NewShard.ShardOption = (FMath::RandRange(0.f, 1.f) > 0.5f ? 0 : 1);
	OutShards.Add(NewShard);
}