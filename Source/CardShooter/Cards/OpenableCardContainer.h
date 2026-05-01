#pragma once

#include "CardData.h"
#include "Engine/DataTable.h"

#include "OpenableCardContainer.generated.h"

UCLASS(BlueprintType)
class UOpenableCardContainerInstance : public UObject
{
	GENERATED_BODY()
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Draw")
	float Weight = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CardData")
	TObjectPtr<UCardData> Card;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CardData")
	ECardRarity CardRarity = ECardRarity::Common;

	//TODO here come more properties that more closely describe the actual card identity, like foil and what other customization we offer

	bool operator== (const FDrawableCardData& Other) const
	{
		return Card == Other.Card
			&& CardRarity == Other.CardRarity;
	}
};

FORCEINLINE uint32 GetTypeHash(const FDrawableCardData& Data)
{
	uint32 Hash = 0;

	Hash = HashCombine(Hash, GetTypeHash(Data.Card));
	Hash = HashCombine(Hash, GetTypeHash(Data.CardRarity));

	return Hash;
}

UCLASS(BlueprintType)
class UOpenableCardContainerDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText ContainerName;

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
};