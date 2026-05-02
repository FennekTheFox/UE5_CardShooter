#pragma once

#include "CardData.h"
#include "Engine/DataTable.h"

#include "OpenableCardContainer.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCardPack, Log, Log)

UCLASS(BlueprintType)
class UOpenableCardContainerInstance : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<FCardIdentity> Cards;
};

USTRUCT(BlueprintType)
struct FCardDrawProperties
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<int32> CardPools;
};

USTRUCT(BlueprintType)
struct FDrawableCardData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Weight = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCardIdentity Card;
};

UCLASS(BlueprintType)
class UOpenableCardContainerDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText ContainerName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 PackOrderIndex = 999;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UTexture2D* ContainerTexture;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTagQuery SaveDataPurchaseCondition;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTagQuery SaveDataVisibilityCondition;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FCardDrawProperties> CardDraws;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (RowType = "/Script/CardShooter.FDrawableCardData"))
	TArray<TObjectPtr<UDataTable>> CardDrawPools;

	UFUNCTION(BlueprintCallable)
	UOpenableCardContainerInstance* GenerateInstance(UObject* Outer);
};