#include "OpenableCardContainer.h"

DEFINE_LOG_CATEGORY(LogCardPack)

UOpenableCardContainerInstance* UOpenableCardContainerDefinition::GenerateInstance(UObject* Outer)
{
	UOpenableCardContainerInstance* NewInst = NewObject<UOpenableCardContainerInstance>(Outer);

	UE_LOG(LogCardPack, Log, TEXT("======= Generating new card pack ========="));

	UE_LOG(LogCardPack, Log, TEXT("Iterating through draws..."));

	int32 DrawCount = 1;
	//Iterate through draws
	for (const FCardDrawProperties& Draw : CardDraws)
	{
		UE_LOG(LogCardPack, Log, TEXT(" = > Draw number %d"), DrawCount);
		DrawCount++;

		float Random = FMath::FRandRange(0.f, 1.f);
		UE_LOG(LogCardPack, Log, TEXT(" === > Random value got: %f"), Random);


		UE_LOG(LogCardPack, Log, TEXT(" ==== > Summing up weights..."));
		//Sum up the weights
		float SumWeights = GetSummedUpDrawWeightsForDraw(Draw);
		//done summing up!
		UE_LOG(LogCardPack, Log, TEXT(" ====== > Sum of weights value got: %f"), SumWeights);


		float TargetValue = SumWeights * Random;

		UE_LOG(LogCardPack, Log, TEXT(" ======= > Target value got: %f"), TargetValue);
		UE_LOG(LogCardPack, Log, TEXT(" ======== > Counting down target value..."));
		for (int32 PoolIndex : Draw.CardPools)
		{
			if (!ensure(CardDrawPools.IsValidIndex(PoolIndex)) && CardDrawPools[PoolIndex].PoolTable)
			{
				continue;
			}

			const FCardDrawPool& DrawPool = CardDrawPools[PoolIndex];
			const TMap<FName, uint8*>& RowMap = DrawPool.PoolTable->GetRowMap();

			//Iterate through to sum up weights
			for (const TPair<FName, uint8*>& Pair : RowMap)
			{
				if (TargetValue < 0.f)
					break;

				FName RowName = Pair.Key;

				FDrawableCardData* Row = (FDrawableCardData*)Pair.Value;

				if (Row)
				{
					TargetValue -= DrawPool.TableWeightMultiplier * Row->Weight;
					UE_LOG(LogCardPack, Log, TEXT(" ========= > Target value decreased to: %f"), TargetValue);

					if (TargetValue <= 0.f)
					{
						UE_LOG(LogCardPack, Log, TEXT(" ========== > Card Added."));
						NewInst->Cards.Add(Row->Card);
					}
				}
			}
		}
	}

	return NewInst;
}

float UOpenableCardContainerDefinition::GetSummedUpDrawWeightsForDraw(const FCardDrawProperties& DrawProperties) const
{
	float SumWeights = 0.f;

	for (int32 PoolIndex : DrawProperties.CardPools)
	{
		if (!ensure(CardDrawPools.IsValidIndex(PoolIndex)) && CardDrawPools[PoolIndex].PoolTable)
		{
			continue;
		}

		const FCardDrawPool& DrawPool = CardDrawPools[PoolIndex];
		const TMap<FName, uint8*>& RowMap = DrawPool.PoolTable->GetRowMap();

		//Iterate through to sum up weights
		for (const TPair<FName, uint8*>& Pair : RowMap)
		{
			FName RowName = Pair.Key;

			FDrawableCardData* Row = (FDrawableCardData*)Pair.Value;

			if (Row)
			{
				SumWeights += DrawPool.TableWeightMultiplier * Row->Weight;
				UE_LOG(LogCardPack, Log, TEXT(" ===== > Sum of weights increased to: %f"), SumWeights);
			}
		}
	}

	return SumWeights;
}
