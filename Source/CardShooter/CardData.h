#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"

#include "CardData.generated.h"

class UTexture2D;

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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Execute(APawn* ExecutingPawn);
};

UCLASS(BlueprintType, Blueprintable)
class UCardEffectFragment_Execution_GameplayAbility : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayAbilitySpecDef AbilityToExecute;

	void Execute_Implementation(APawn* ExecutingPawn) override;

};


UCLASS(BlueprintType, Blueprintable)
class UCardData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
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
};