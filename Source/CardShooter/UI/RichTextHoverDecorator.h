#pragma once

#include "CoreMinimal.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateTypes.h"
#include "Framework/Text/TextLayout.h"
#include "Framework/Text/ISlateRun.h"
#include "Framework/Text/ITextDecorator.h"
#include "Components/RichTextBlockDecorator.h"
#include "Engine/DataTable.h"

#include "RichTextHoverDecorator.generated.h"

class ISlateStyle;
class UInputAction;
class URichTextBlock;

UINTERFACE(BlueprintType)
class CARDSHOOTER_API UHoverableContext : public UInterface
{
	GENERATED_BODY()
};

class CARDSHOOTER_API IHoverableContext
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HoverableContext")
	void GetHoverText(FText& OutText) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HoverableContext")
	void GetHoverContentTitle(FText& OutText) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "My Interface")
	void GetHoverContent(FText& OutText) const;
};

/**
 * Allows you to setup an image decorator that can be configured
 * to map certain input actions to certain images, depending on the platform
 *
 * Understands the format <img id="NameOfBrushInTable"></>
 */
UCLASS(Abstract, Blueprintable, MinimalAPI)
class URichTextBlockHoverObjectDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tooltip")
	TSubclassOf<UUserWidget> TooltipWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Hover")
	FTextBlockStyle DefaultTextStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Hover")
	TObjectPtr<UDataTable> TextStyleSet;

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;

};
