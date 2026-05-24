#pragma once

#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Cards/Augments/CardAugmentData.h"

#include "CardExecution_AdditionalData.generated.h"


UENUM(BlueprintType)
enum class ESetByCallerMagnitudeDisplayType : uint8
{
	Flat = 0,
	Percent = 1,
	PercentOneMinus = 2,
	Absolute,
	AbsolutePercent,
	AbsolutePercentOneOver,
	XOver100,
	Rotation,
};

USTRUCT(BlueprintType)
struct FSetByCallerMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Magnitude = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ScalingGroup;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESetByCallerMagnitudeDisplayType DisplayType = ESetByCallerMagnitudeDisplayType::Flat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ParameterName;

	FText GetDisplayText() const;
};


USTRUCT(BlueprintType)
struct FReplicatedTargetData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag TargetDataID;

	UPROPERTY(BlueprintReadWrite)
	FGameplayAbilityTargetDataHandle TargetData;
};

USTRUCT(BlueprintType)
struct FCardExecution_AdditionalData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 EchoIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 ComboIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	TArray<FReplicatedTargetData> TargetData;

	UPROPERTY(BlueprintReadWrite)
	TArray<FCardAugmentData> AugmentData;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayTag> SetByCallerMetaDataTags;

	UPROPERTY(BlueprintReadOnly)
	TArray<FSetByCallerMetaData> SetByCallerMetaDataValues;
};