#pragma once

#include "GameplayEffect.h"
#include "UI/RichTextHoverDecorator.h"

#include "GameplayEffect_CardExecution.generated.h"

UCLASS(BlueprintType, Blueprintable, Abstract)
class UGameplayEffect_CardExecution : public UGameplayEffect,
	public IHoverableContext
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
	FText GetDescription(const FCardIdentity& CardIdentity);

	FText GetDescription(const FCardIdentity& CardIdentity, class UCardEffectFragment_Execution_WithParameters* OwningCEF_E_WP);

	UFUNCTION(BlueprintPure)
	class UTexture2D* GetIcon() 
	{
		return EffectIcon;
	}


	void GetHoverContentTitle_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const UCardEffectFragment_Execution_WithParameters* RelatedExec = nullptr) const override;

};