#pragma once

#include "CardAttributeFragment_Execution.h"

#include "CardEffectFragment_EffectToSelf.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Apply Effect To Self"))
class UCardEffectFragment_Execution_ApplyEffectToOwner : public UCardEffectFragment_Execution
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float GameplayEffectLevel = 1.f;

	//The tag applied by the gameplay effect to track when the persistent effect gets removed
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag TrackingTag;

	virtual void ExecuteCardEffect_Implementation(APawn* ExecutingPawn, const FCardExecution_AdditionalData& AdditionalData) override;

	//BEGIN UCardExecutionInfo
	FText GetName_Implementation() override;
	//BEGIN UCardExecutionInfo
};