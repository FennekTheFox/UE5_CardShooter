#pragma once

#include "CardAttributeFragment_Execution.h"
#include "Internationalization/Text.h"

#include "CardEffectFragment_EffectToSelf.generated.h"

UENUM(BlueprintType)
enum class ESetByCallerMagnitudeDisplayType : uint8
{
	Flat = 0,
	Percent = 1, 
	PercentOneMinus = 2
};

//Only used for text generation
UENUM(BlueprintType)
enum class EEffectInclination : uint8
{
	Benefitial = 0,
	Detrimental = 1,
};

USTRUCT(BlueprintType)
struct FSetByCallerMetaData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Magnitude = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESetByCallerMagnitudeDisplayType DisplayType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ParameterName;

	FText GetDisplayText() const;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Apply Effect To Self"))
class UCardEffectFragment_ApplyEffectToOwner : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEffectInclination EffectInclination = EEffectInclination::Benefitial;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float GameplayEffectLevel = 1.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TMap<FGameplayTag, FSetByCallerMetaData> SetByCallerMagnitudes;

	virtual void ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData) override;

	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment

	void GetSetByCallerFormattingArgs(FFormatNamedArguments& Args);
};