#pragma once

#include "GameplayEffect.h"

#include "GameplayEffect_CardExecution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class UGameplayEffect_CardExecution : public UGameplayEffect
{
	GENERATED_BODY()

	UGameplayEffect_CardExecution();

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText SimpleEffectName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText DetailedEffectName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText EffectDesc;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UTexture2D* EffectIcon;

	UFUNCTION(BlueprintPure)
	FText GetDetailedLocalizableName() const
	{
		return DetailedEffectName;
	}

	//Name that does not include magnitudes in it
	UFUNCTION(BlueprintPure)
	FText GetSimpleLocalizableName() const
	{
		return SimpleEffectName;
	}

	UFUNCTION(BlueprintPure)
	FText GetDescription() 
	{
		return EffectDesc;
	}

	FText GetDescription(class UCardEffectFragment_ApplyEffectToOwner* OwningCEF_AETO);

	UFUNCTION(BlueprintPure)
	class UTexture2D* GetIcon() 
	{
		return EffectIcon;
	}

};