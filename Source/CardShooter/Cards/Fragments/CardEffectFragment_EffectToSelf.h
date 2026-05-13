#pragma once

#include "CardAttributeFragment_Execution.h"
#include "Internationalization/Text.h"

#include "CardEffectFragment_EffectToSelf.generated.h"

//Only used for text generation
UENUM(BlueprintType)
enum class EEffectInclination : uint8
{
	Benefitial = 0,
	Detrimental = 1,
};

UCLASS()
class UCardExecutionInstance_ApplyEffectToOwner : public UCardExecutionInstance
{
	GENERATED_BODY()

public:
	void ProcessExecutionInstance() override;
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Apply Effect To Self"))
class UCardEffectFragment_ApplyEffectToOwner : public UCardEffectFragment_ExecutionWithParameters
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
	bool bResolveImmediately = false;

	UCardExecutionInstance* ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData) override;

	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment
};