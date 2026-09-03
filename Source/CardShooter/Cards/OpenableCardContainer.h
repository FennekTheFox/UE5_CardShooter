#pragma once

#include "CardData.h"
#include "Engine/DataTable.h"

#include "OpenableCardContainer.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCardPack, Log, Log)

UCLASS(Blueprintable, BlueprintType, Abstract)
class UCardIdentityModifier : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, BlueprintImplementableEvent)
	FText GetModifierDescription() const;

	UFUNCTION(BlueprintImplementableEvent)
	void ModifyCard(UPARAM(ref) FCardIdentity& CardToModify) const;
};


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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UCardIdentityModifier*> Modifiers;
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

USTRUCT(BlueprintType)
struct FCardDrawPool
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TableWeightMultiplier = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> PoolTable;
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
	TMap<TObjectPtr<class UCurrencyDef>, int32> BaseCurrencyCost;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<FCardDrawProperties> CardDraws;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (RowType = "/Script/CardShooter.FDrawableCardData"))
	TArray<FCardDrawPool> CardDrawPools;

	UFUNCTION(BlueprintCallable)
	UOpenableCardContainerInstance* GenerateInstance(UObject* Outer);

	UFUNCTION(BlueprintPure)
	float GetSummedUpDrawWeightsForDraw(const FCardDrawProperties& DrawProperties) const;
};