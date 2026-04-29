#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Gameplay/ICardExecutionInfo.h"

#include "CardData.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FCardIdentity
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	FGameplayTag AssetTag;

	//This is supposed to make the access to the card faster. If not found because the asset is moved
	//we need to look through all card data assets to find the right one
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	FSoftObjectPath CardDataPath;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	int32 UniqueID;

	//TODO here come more properties that more closely describe the actual card identity, like foil and what other customization we offer

	bool operator== (const FCardIdentity& Other)
	{
		return AssetTag == Other.AssetTag && UniqueID == Other.UniqueID;
	}
};

USTRUCT(BlueprintType)
struct FCardIdentityList
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FCardIdentity> Cards;
};

class UCardEffectFragment_Attribute;
class UCardEffectFragment_Execution;

UCLASS(BlueprintType, Blueprintable)
class UCardData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	//This tag needs to be set in order for the utility functions to be able to load the card from the tag alone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Gameplay.Cards.IDs"))
	FGameplayTag AssetTag;

	//This tag needs to be set in order for the utility functions to be able to load the card from the tag alone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer CardTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CardName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> CardIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> CardTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UCardEffectFragment_Attribute>> CardAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UCardEffectFragment_Execution> PrimaryExecution;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UCardEffectFragment_Execution> SecondaryExecution;

public:
	UFUNCTION(BlueprintPure)
	void GetIdentity(FCardIdentity& Identity);
	UFUNCTION(BlueprintPure)
	TArray<UCardEffectFragment_Attribute*> GetAllAttributesOfClass(TSubclassOf<UCardEffectFragment_Attribute> Class);
	UFUNCTION(BlueprintPure)
	UCardEffectFragment_Attribute* GetAttributeOfClass(TSubclassOf<UCardEffectFragment_Attribute> Class);

	UFUNCTION(BlueprintPure)
	FText GetDescription();
};