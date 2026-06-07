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
class UCardEffectFragment_ApplyEffectToOwner : public UCardEffectFragment_Execution_WithParameters
{
	GENERATED_BODY()

	UCardEffectFragment_ApplyEffectToOwner();

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	EEffectInclination EffectInclination = EEffectInclination::Benefitial;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
	float GameplayEffectLevel = 1.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effect")
	bool bResolveImmediately = false;

	 //~UCardEffectFragment
	FText GetLocalizableDescription_Implementation(const FCardIdentity& CardIdentity) const override;
	FText GetRawLocalizableDescription(const FCardIdentity& CardIdentity) const override;
	 //~UCardEffectFragment
};