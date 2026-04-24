#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"

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

	//TODO here come more properties that more closely describe the actual card identity, like foil and what other customization we offer
};



UCLASS(BlueprintType, EditInlineNew)
class UCardEffectFragment_Base : public UObject
{
	GENERATED_BODY()

public:
	//Returns the "label" of the fragment
	UFUNCTION(BlueprintPure)
	FText GetFragmentLabel();
};


//The base class for all root level fragments
//Supports returning a description
UCLASS(BlueprintType)
class UCardEffectFragment_Root : public UCardEffectFragment_Base
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FText GetFragmentDescription();
};


UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_Execution : public UCardEffectFragment_Root
{
	GENERATED_BODY()

public:
	//Execution of cards always needs to happen on server side, and be replicated to the client
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ExecuteCardEffect(APawn* ExecutingPawn);
};

UCLASS(BlueprintType, Blueprintable)
class UCardEffectFragment_Execution_GameplayAbility : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayAbilitySpecDef AbilityToExecute;

	void Server_ExecuteCardEffect_Implementation(APawn* ExecutingPawn) override;

};


UCLASS(BlueprintType, Blueprintable)
class UCardData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	//This tag needs to be set in order for the utility functions to be able to load the card from the tag alone
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(Categories="Gameplay.Cards.IDs"))
	FGameplayTag AssetTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CardName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CardDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> CardIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> CardTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<TObjectPtr<UCardEffectFragment_Root>> CardAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UCardEffectFragment_Execution> PrimaryExecution;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TObjectPtr<UCardEffectFragment_Execution> SecondaryExecution;


	UFUNCTION(BlueprintPure)
	void GetIdentity(FCardIdentity& Identity);
};