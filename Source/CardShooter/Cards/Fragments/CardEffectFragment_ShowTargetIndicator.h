#pragma once

#include "CardAttributeFragment_Base.h"
#include "Abilities/GameplayAbilityTargetTypes.h"

#include "CardEffectFragment_ShowTargetIndicator.generated.h"

UENUM(BlueprintType)
enum class ETargetIndicatorType : uint8
{
	None,
	CircleUnderCrosshair,
};

UCLASS(BlueprintType, Blueprintable, Abstract)
class ATargetVisualizer : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta=  (ExposeOnSpawn))
	TObjectPtr<class UCardEffectFragment_Targeting> TargetingAttr;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Update(UPARAM()const FGameplayAbilityTargetDataHandle& TargetData);
};

UCLASS(BlueprintType, meta = (DisplayName = "Targeting"))
class UCardEffectFragment_Targeting : public UCardEffectFragment_Attribute
{
	GENERATED_BODY()

	UCardEffectFragment_Targeting();


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<ATargetVisualizer> TargetVisualizerActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ETargetIndicatorType IndicatorType = ETargetIndicatorType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Range = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Radius = 20.f;
};