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

#include "RichTextHoverDecorator.generated.h"

class ISlateStyle;
class UInputAction;
class URichTextBlock;

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
	UPROPERTY(EditDefaultsOnly)
	FTextBlockStyle TextStyle;

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;

};
