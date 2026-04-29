#pragma once

#include "CardAttributeFragment_Base.h"
#include "GameplayTagContainer.h"

#include "CardEffectFragment_Persistent.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Persistent"))
class UCardEffectFragment_Persistent : public UCardEffectFragment_ExecutionAttribute
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTag TrackingTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<class UGameplayEffect_CardExecution> TrackedEffect;

	 //~UCardEffectFragment
	FText GetLocalizableName_Implementation() override;
	FText GetLocalizableDescription_Implementation() override;
	 //~UCardEffectFragment
};