#pragma once

#include "GameFramework/DamageType.h"
#include "UI/RichTextHoverDecorator.h"

#include "NeonDamageType.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UNeonDamageType : public UDamageType,
	public IHoverableContext
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText DamageTypeName = FText::FromString("Unnamed damage type");

	UPROPERTY(EditDefaultsOnly)
	FText RichStyle = FText::FromString("damagetypephysical");

	UPROPERTY(EditDefaultsOnly)
	FText DamageTypeDescription = FText::FromString("This damage type doesnt have a description yet.");

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSubclassOf<UObject>> HoverContextObjects;

	void GetHoverText_Implementation(FText& OutText) const override;
	void GetHoverContentTitle_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const UCardEffectFragment_Execution_WithParameters* RelatedExec = nullptr) const override;
	void GetHoverContent_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const FCardIdentity& CardIdentity, const UCardEffectFragment_Execution_WithParameters* RelatedExec = nullptr) const override;

	virtual void GetHoverStyle(FText& OutText) const override { OutText = RichStyle; };
	void GetFormattingArgs(const UCardEffectFragment_Execution_WithParameters* RelatedExec, FFormatNamedArguments& FormattingArguments) const override;

};