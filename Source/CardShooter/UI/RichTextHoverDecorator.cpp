#include "RichTextHoverDecorator.h"

#include "Components/RichTextBlock.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/Package.h"
#include "Kismet/GameplayStatics.h"
#include "Cards/CardAttributeFragment_Base.h"
#include "Gameplay/CardExecutionEffect.h"
#include "Cards/CardEffectFragment_EffectToSelf.h"
#include "Widgets/Layout/SBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RichTextHoverDecorator)


#define LOCTEXT_NAMESPACE "UMG"
class SHoverObjectWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHoverObjectWidget) {}
		SLATE_ARGUMENT(TObjectPtr<UObject>, HoverObject)
		SLATE_ARGUMENT(TObjectPtr<UObject>, RelatedEffectExecFrag)
		SLATE_ATTRIBUTE(FTextBlockStyle, TextStyle)
		SLATE_ATTRIBUTE(FText, Text)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		HoverObject = InArgs._HoverObject;
		RelatedEffectExecFrag = InArgs._RelatedEffectExecFrag;
		TextStyle = InArgs._TextStyle;
		Text = InArgs._Text;

		ChildSlot
			[
				SNew(STextBlock)
					.Text(this, &SHoverObjectWidget::GetText)
					.TextStyle(&TextStyle.Get())
			];
	}

private:

	FText GetText() const
	{
		return Text.Get();
	}

private:
	TObjectPtr<UObject> HoverObject;
	TObjectPtr<UObject> RelatedEffectExecFrag;
	TAttribute<FTextBlockStyle> TextStyle;
	TAttribute<FText> Text;

	TSharedPtr<SWindow> TooltipWindow;
	TSharedPtr<SWidget> TooltipContent;;


	FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		const FVector2D CursorPos = MouseEvent.GetScreenSpacePosition();

		if (TooltipWindow.IsValid())
		{
			TooltipWindow->MoveWindowTo(CursorPos + FVector2D(25.f, 25.f));
		}

		return FReply::Handled();
	}


	void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		FText Content = FText::FromString("Couldn't parse context path.");
		if (HoverObject)
		{
			if (UCardEffectFragment_Attribute* AsAttribute = Cast<UCardEffectFragment_Attribute>(HoverObject))
			{
				Content = AsAttribute->GetLocalizableDescription();
			}else		
			if (UGameplayEffect_CardExecution* AsEffect = Cast<UGameplayEffect_CardExecution>(HoverObject))
			{
				if (auto* AsCEF_AETO = Cast<UCardEffectFragment_ApplyEffectToOwner>(RelatedEffectExecFrag))
				{
					Content = AsEffect->GetDescription(AsCEF_AETO);
				}
				else
				{
					Content = AsEffect->GetDescription();
				}
			}
		}

		TooltipContent =
			SNew(SBox)
			.MaxDesiredWidth(300.f)
			[
				SNew(SBorder)
					.Padding(FMargin(8.f))
					[
						SNew(STextBlock)
							.Text(Content)
							.AutoWrapText(true)
					]
			];

		TooltipWindow =
			SNew(SWindow)
			.CreateTitleBar(false)
			.SizingRule(ESizingRule::Autosized)
			.ClientSize(FVector2D(200, 80))
			[
				TooltipContent.ToSharedRef()
			];

		FSlateApplication::Get().AddWindow(TooltipWindow.ToSharedRef());
	}


	void OnMouseLeave(const FPointerEvent& MouseEvent) override
	{
		if (TooltipWindow.IsValid())
		{
			TooltipWindow->RequestDestroyWindow();
			TooltipWindow.Reset();
		}
	}
};



class FHoverObjectTextDecorator : public FRichTextDecorator
{
private:
	FTextBlockStyle DecoratorTextStyle;

public:
	FHoverObjectTextDecorator(URichTextBlock* InOwner, FTextBlockStyle InTextStyle)
		: FRichTextDecorator(InOwner)
		, DecoratorTextStyle(InTextStyle)
	{
	}

	virtual bool Supports(const FTextRunParseResults& Run, const FString& Text) const override
	{
		return Run.Name == TEXT("hover")
			&& Run.MetaData.Contains(TEXT("id"));
	}

	virtual TSharedPtr<SWidget> CreateDecoratorWidget(
		const FTextRunInfo& RunInfo,
		const FTextBlockStyle& TextStyle) const override
	{
		const FString* HoveredObjectPath = RunInfo.MetaData.Find(TEXT("id"));
		const FString* EffectObjectPath = RunInfo.MetaData.Find(TEXT("relatedEffectExec"));

		if (!HoveredObjectPath || HoveredObjectPath->IsEmpty())
		{
			return SNew(STextBlock).Text(FText::FromString(TEXT("")));
		}

		UObject* Obj = LoadObject<UObject>(nullptr, **HoveredObjectPath);
		UObject* Eff = nullptr;

		if (EffectObjectPath)
		{
			FString OuterPath;
			FString SubObjectName;
			EffectObjectPath->Split(TEXT(":"), &OuterPath, &SubObjectName);

			UObject* Outer = LoadObject<UObject>(nullptr, *OuterPath);

			if (Outer)
			{
				Eff = FindObject<UObject>(Outer, *SubObjectName);
			}
		}


		return SNew(SHoverObjectWidget)
			.HoverObject(Obj)
			.RelatedEffectExecFrag(Eff)
			.TextStyle(DecoratorTextStyle)
			.Text(RunInfo.Content);
	}
};

//=============================
TSharedPtr<ITextDecorator> URichTextBlockHoverObjectDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FHoverObjectTextDecorator(InOwner, TextStyle));
}

#undef LOCTEXT_NAMESPACE