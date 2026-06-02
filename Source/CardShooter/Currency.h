#pragma once

#include "GameplayTagContainer.h"

#include "Currency.generated.h"


UCLASS(BlueprintType, Blueprintable)
class UCurrencyCategory : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText CategoryName;
};

UCLASS(BlueprintType, Blueprintable)
class UCurrencyDef : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag CurrencyTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCurrencyCategory> Category;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText CurrencyName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText CurrencyDescription;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<class UTexture2D> CurrencyIcon;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, AssetRegistrySearchable)
	FString CommandString;
};