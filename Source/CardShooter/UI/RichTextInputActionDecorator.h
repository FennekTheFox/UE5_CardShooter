#pragma once

#include "CoreMinimal.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateTypes.h"
#include "Framework/Text/TextLayout.h"
#include "Framework/Text/ISlateRun.h"
#include "Framework/Text/ITextDecorator.h"
#include "Components/RichTextBlockDecorator.h"
#include "Engine/DataTable.h"
#include "VREditorInteractor.h"

#include "RichTextInputActionDecorator.generated.h"

class ISlateStyle;
class UInputAction;
class URichTextBlock;

UENUM(BlueprintType)
enum class EInputDeviceType : uint8
{
	None,
	KBM,
	Gamepad
};

USTRUCT()
struct FControllerInputIcons
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<class UInputAction*, FSlateBrush> IconMapping;
};

USTRUCT()
struct FRichInputActionDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<EInputDeviceType, FControllerInputIcons> ControllerData;
};

/**
 * Allows you to setup an image decorator that can be configured
 * to map certain input actions to certain images, depending on the platform
 *
 * Understands the format <img id="NameOfBrushInTable"></>
 */
UCLASS(Abstract, Blueprintable, MinimalAPI)
class URichTextBlockInputActionDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;

	virtual const FSlateBrush* FindInputActionBrush(FName Platform, UInputAction* InputAction, bool bWarnIfMissing, URichTextBlock* InOwner);

protected:

	FRichInputActionDataRow* FindInputActionBrushRow(FName Platform, bool bWarnIfMissing, URichTextBlock* InOwner);

	UPROPERTY(EditAnywhere, Category = Appearance/*, meta = (RequiredAssetDataTags = "RowStructure=/Script/UMG.RichInputActionRow")*/)
	TObjectPtr<class UDataTable> ImageSet;
};
