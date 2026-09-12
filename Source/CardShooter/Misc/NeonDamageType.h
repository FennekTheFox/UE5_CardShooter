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

	void GetHoverText_Implementation(FText& OutText) const override;
	void GetHoverContentTitle_Implementation(FText& OutText) const override;
	void GetHoverContent_Implementation(FText& OutText) const override;

	virtual void GetHoverStyle(FText& OutText) const override { OutText = RichStyle; };
};