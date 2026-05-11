#include "RichTextInputActionDecorator.h"

#include "Components/RichTextBlock.h"
#include "Actors/CardShooterPlayerController.h"
#include "UObject/SoftObjectPtr.h"
#include "Rendering/DrawElements.h"
#include "Framework/Text/SlateTextRun.h"
#include "Framework/Text/SlateTextLayout.h"
#include "Slate/SlateGameResources.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Framework/Application/SlateApplication.h"
#include "Fonts/FontMeasure.h"
#include "Math/UnrealMathUtility.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SBox.h"
#include "Misc/DefaultValueHelper.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/Package.h"
#include "Kismet/GameplayStatics.h"
#include "InputAction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RichTextInputActionDecorator)


#define LOCTEXT_NAMESPACE "UMG"


class SRichInlineInputAction : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRichInlineInputAction)
		{
		}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const FSlateBrush* Brush, const FTextBlockStyle& TextStyle, TOptional<int32> Width, TOptional<int32> Height, EStretch::Type Stretch)
	{
		check(Brush);

		const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		float IconHeight = FMath::Min((float)FontMeasure->GetMaxCharacterHeight(TextStyle.Font, 1.0f), Brush->ImageSize.Y);

		if (Height.IsSet())
		{
			IconHeight = static_cast<float>(Height.GetValue());
		}

		float IconWidth = IconHeight;
		if (Width.IsSet())
		{
			IconWidth = static_cast<float>(Width.GetValue());
		}

		ChildSlot
			[
				SNew(SBox)
					.HeightOverride(IconHeight)
					.WidthOverride(IconWidth)
					[
						SNew(SScaleBox)
							.Stretch(Stretch)
							.StretchDirection(EStretchDirection::DownOnly)
							.VAlign(VAlign_Center)
							[
								SNew(SImage)
									.Image(Brush)
							]
					]
			];
	}
};


/**
 * Add an input action icon inline with the text.
 * Usage: Before image <input id="FullPackagePath"/>, after image.
 *
 * A width and height can be specified.
 * By default the width and the height is the same size as the font height.
 * Use "desired" to use the same size as the image brush.
 * Usage: Before image <img id="MyId" height="40" width="desired"/>, after image.
 *
 * A stretch type can be specified. See EStretch.
 * By default the stretch type is ScaleToFit.
 * Usage: Before image <img id="MyId" stretch="ScaleToFitY"/>, after image.
 */
class FRichInlineInputAction : public FRichTextDecorator
{
public:
	FRichInlineInputAction(URichTextBlock* InOwner, URichTextBlockInputActionDecorator* InDecorator)
		: FRichTextDecorator(InOwner)
		, Decorator(InDecorator)
	{
		Owner = InOwner;
	}

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		if (RunParseResult.Name == TEXT("input") && RunParseResult.MetaData.Contains(TEXT("id")))
		{
			return true;
		}

		return false;
	}

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle) const override
	{
		const bool bWarnIfMissing = true;

		FName CurrentPlatformName = FName(UGameplayStatics::GetPlatformName());
		
		const FString* IdStr = RunInfo.MetaData.Find(TEXT("id"));
		UInputAction* InputAction = nullptr;
		if (IdStr)		
		{
			InputAction = LoadObject<UInputAction>(nullptr, **IdStr);
		}

		const FSlateBrush* Brush = Decorator->FindInputActionBrush(CurrentPlatformName, InputAction, bWarnIfMissing, Owner.Get());

		//No ensure here, else it triggers in UI
		if (/*ensure*/(Brush))
		{
			TOptional<int32> Width;
			if (const FString* WidthString = RunInfo.MetaData.Find(TEXT("width")))
			{
				int32 WidthTemp;
				if (FDefaultValueHelper::ParseInt(*WidthString, WidthTemp))
				{
					Width = WidthTemp;
				}
				else
				{
					if (FCString::Stricmp(GetData(*WidthString), TEXT("desired")) == 0)
					{
						Width = FMath::TruncToInt32(Brush->ImageSize.X);
					}
				}
			}

			TOptional<int32> Height;
			if (const FString* HeightString = RunInfo.MetaData.Find(TEXT("height")))
			{
				int32 HeightTemp;
				if (FDefaultValueHelper::ParseInt(*HeightString, HeightTemp))
				{
					Height = HeightTemp;
				}
				else
				{
					if (FCString::Stricmp(GetData(*HeightString), TEXT("desired")) == 0)
					{
						Height = FMath::TruncToInt32(Brush->ImageSize.Y);
					}
				}
			}

			EStretch::Type Stretch = EStretch::ScaleToFit;
			if (const FString* SstretchString = RunInfo.MetaData.Find(TEXT("stretch")))
			{
				const UEnum* StretchEnum = StaticEnum<EStretch::Type>();
				int64 StretchValue = StretchEnum->GetValueByNameString(*SstretchString);
				if (StretchValue != INDEX_NONE)
				{
					Stretch = static_cast<EStretch::Type>(StretchValue);
				}
			}

			return SNew(SRichInlineInputAction, Brush, TextStyle, Width, Height, Stretch);
		}
		return TSharedPtr<SWidget>();
	}

private:
	URichTextBlockInputActionDecorator* Decorator;
	TWeakObjectPtr<URichTextBlock> Owner;
};

//=============================


TSharedPtr<ITextDecorator> URichTextBlockInputActionDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FRichInlineInputAction(InOwner, this));
}

const FSlateBrush* URichTextBlockInputActionDecorator::FindInputActionBrush(FName Platform, UInputAction* InputAction, bool bWarnIfMissing, URichTextBlock* InOwner)
{
	const FRichInputActionDataRow* Row = FindInputActionBrushRow(Platform, bWarnIfMissing, InOwner);
	if (Row)
	{
		ACardShooterPlayerController* PC = Cast<ACardShooterPlayerController>(InOwner->GetOwningPlayer());

		if (!PC)
		{
			return nullptr;
		}

		const FControllerInputIcons* ControllerData = Row->ControllerData.Find(PC->bLastInputWasGamepad ? EInputDeviceType::Gamepad : EInputDeviceType::KBM);

		if (ControllerData)
		{
			return ControllerData->IconMapping.Find(InputAction);
		}
	}

	return nullptr;
}

FRichInputActionDataRow* URichTextBlockInputActionDecorator::FindInputActionBrushRow(FName Platform, bool bWarnIfMissing, URichTextBlock* InOwner)
{
	if (ImageSet)
	{
		FString ContextString;
		return ImageSet->FindRow<FRichInputActionDataRow>(Platform, ContextString, bWarnIfMissing);
	}

	return nullptr;
}

#undef LOCTEXT_NAMESPACE