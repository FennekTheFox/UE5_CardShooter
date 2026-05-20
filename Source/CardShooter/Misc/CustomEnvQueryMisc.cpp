#include "CustomEnvQueryMisc.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "AIController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CustomEnvQueryMisc)

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEnvQueryGenerator_SimpleGridOnSide::UEnvQueryGenerator_SimpleGridOnSide(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GenerateAround = UEnvQueryContext_Querier::StaticClass();
	GridSize.DefaultValue = 500.0f;
	SpaceBetween.DefaultValue = 100.0f;
}

void UEnvQueryGenerator_SimpleGridOnSide::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	UObject* BindOwner = QueryInstance.Owner.Get();
	GridSize.BindData(BindOwner, QueryInstance.QueryID);
	SpaceBetween.BindData(BindOwner, QueryInstance.QueryID);
	bOnRightSide.BindData(BindOwner, QueryInstance.QueryID);

	float RadiusValue = GridSize.GetValue();
	float DensityValue = SpaceBetween.GetValue();

	const int32 ItemCount = FPlatformMath::TruncToInt((RadiusValue * 2.0f / DensityValue) + 1);
	const int32 ItemCountHalf = ItemCount / 2;

	TArray<FVector> ContextLocations;
	QueryInstance.PrepareContext(GenerateAround, ContextLocations);

	TArray<FNavLocation> GridPoints;
	GridPoints.Reserve(ItemCount * ItemCount * ContextLocations.Num());

	AActor* QuerierActor = Cast<AActor>(BindOwner);

	if (const AAIController* AIController = Cast<AAIController>(BindOwner))
	{
		QuerierActor = AIController->GetPawn();
	}

	if (!QuerierActor)
	{
		return;
	}

	const FVector QuerierRight = QuerierActor->GetActorRightVector().GetSafeNormal2D();

	for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
	{
		const FVector ContextLocation = ContextLocations[ContextIndex];

		for (int32 IndexX = 0; IndexX < ItemCount; ++IndexX)
		{
			for (int32 IndexY = 0; IndexY < ItemCount; ++IndexY)
			{
				const FNavLocation TestPoint = FNavLocation(ContextLocations[ContextIndex] - FVector(DensityValue * (IndexX - ItemCountHalf), DensityValue * (IndexY - ItemCountHalf), 0));
				
				const FVector TestLocation =
					ContextLocation -
					FVector(
						DensityValue * (IndexX - ItemCountHalf),
						DensityValue * (IndexY - ItemCountHalf),
						0.0f
					);

				const FVector DirectionToPoint =
					(TestLocation - ContextLocation).GetSafeNormal2D();

				const float SideDot = FVector::DotProduct(QuerierRight, DirectionToPoint);

				const bool bPointIsOnRightSide = SideDot > 0.0f;

				if (bPointIsOnRightSide == bOnRightSide.GetValue())
				{
					GridPoints.Add(FNavLocation(TestLocation));
				}
			}
		}
	}

	ProjectAndFilterNavPoints(GridPoints, QueryInstance);
	StoreNavPoints(GridPoints, QueryInstance);
}

FText UEnvQueryGenerator_SimpleGridOnSide::GetDescriptionTitle() const
{
	return FText::Format(LOCTEXT("SimpleGridDescriptionGenerateAroundContextOnSide", "{0}: generate around {1}"),
		Super::GetDescriptionTitle(), UEnvQueryTypes::DescribeContext(GenerateAround));
};

FText UEnvQueryGenerator_SimpleGridOnSide::GetDescriptionDetails() const
{
	FText Desc = FText::Format(LOCTEXT("SimpleGridOnSideDescription", "radius: {0}, space between: {1}"),
		FText::FromString(GridSize.ToString()), FText::FromString(SpaceBetween.ToString()));

	FText ProjDesc = ProjectionData.ToText(FEnvTraceData::Brief);
	if (!ProjDesc.IsEmpty())
	{
		FFormatNamedArguments ProjArgs;
		ProjArgs.Add(TEXT("Description"), Desc);
		ProjArgs.Add(TEXT("ProjectionDescription"), ProjDesc);
		Desc = FText::Format(LOCTEXT("SimpleGridDescriptionWithProjection", "{Description}, {ProjectionDescription}"), ProjArgs);
	}

	return Desc;
}

#undef LOCTEXT_NAMESPACE

