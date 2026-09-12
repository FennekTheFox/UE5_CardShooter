#include "RichTextHoverDecorator.h"

#include "Components/RichTextBlock.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Text/SRichTextBlock.h"

#include "UObject/UObjectGlobals.h"
#include "UObject/Package.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/SlateBlueprintLibrary.h"

#include "Cards/CardIdentity.h"
#include "Cards/Fragments/CardAttributeFragment_Base.h"
#include "Cards/Fragments/CardEffectFragment_EffectToSelf.h"
#include "Gameplay/GameplayEffect_CardExecution.h"

#include "InputCoreTypes.h"
#include "Cards/Fragments/CardEffectFragment_GameplayAbility.h"
#include "Util/CardShooterUtility.h"
#include "Cards/Fragments/CardAttributeFragment_Execution.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RichTextHoverDecorator)

class SHoverObjectWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SHoverObjectWidget) {}
		SLATE_ARGUMENT(TObjectPtr<UObject>, HoverObject)
		SLATE_ARGUMENT(TObjectPtr<UObject>, relatedExecFrag)
		SLATE_ARGUMENT(TSubclassOf<UUserWidget>, TooltipWidgetClass)
		SLATE_ARGUMENT(TWeakObjectPtr<UWorld>, World)
		SLATE_ATTRIBUTE(FTextBlockStyle, TextStyle)
		SLATE_ATTRIBUTE(FText, Text)
		SLATE_ATTRIBUTE(FCardIdentity, CardIdentity)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		HoverObject = InArgs._HoverObject;
		relatedExecFrag = InArgs._relatedExecFrag;
		TooltipWidgetClass = InArgs._TooltipWidgetClass;
		World = InArgs._World;
		TextStyle = InArgs._TextStyle;
		Text = InArgs._Text;
		CardIdentity = InArgs._CardIdentity;

		ChildSlot
			[
				SNew(SRichTextBlock)
					.Text(this, &SHoverObjectWidget::GetText)
					.TextStyle(&TextStyle.Get())
			]
			.SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);
	}

	~SHoverObjectWidget()
	{
		RemoveTooltip();
	}

private:
	TObjectPtr<UObject> HoverObject = nullptr;
	TObjectPtr<UObject> relatedExecFrag = nullptr;

	TSubclassOf<UUserWidget> TooltipWidgetClass;
	TWeakObjectPtr<UWorld> World;

	TAttribute<FTextBlockStyle> TextStyle;
	TAttribute<FText> Text;
	TAttribute<FCardIdentity> CardIdentity;

	TWeakObjectPtr<UUserWidget> TooltipUserWidget;

	bool bTooltipFrozen = false;

private:
	FText GetText() const
	{
		return Text.Get();
	}

	FText BuildTooltipContent() const
	{
		FText Content = FText::FromString(TEXT("Couldn't parse context path."));

		if (!HoverObject)
		{
			return Content;
		}

		if (UCardEffectFragment_Attribute* AsCEF_A = Cast<UCardEffectFragment_Attribute>(HoverObject))
		{
			return AsCEF_A->GetLocalizableDescription(CardIdentity.Get());
		}

		if (UGameplayEffect_CardExecution* AsGA_CE = Cast<UGameplayEffect_CardExecution>(HoverObject))
		{
			if (UCardEffectFragment_ApplyEffectToOwner* AsCEF_AETO = Cast<UCardEffectFragment_ApplyEffectToOwner>(relatedExecFrag))
			{
				return AsGA_CE->GetDescription(CardIdentity.Get(), AsCEF_AETO);
			}
			else if (UCardEffectFragment_GameplayAbility* AsCEF_GA = Cast<UCardEffectFragment_GameplayAbility>(relatedExecFrag))
			{
				return AsGA_CE->GetDescription(CardIdentity.Get(), AsCEF_GA);
			}

			return AsGA_CE->GetDescription(CardIdentity.Get());
		}

		if (HoverObject->GetClass()->ImplementsInterface(UHoverableContext::StaticClass()))
		{
			IHoverableContext::Execute_GetHoverContent(HoverObject, Content);

			FCardIdentity TheCardIdentity = CardIdentity.Get();

			if (UCardEffectFragment_Execution_WithParameters* AsCEF_E_WP = Cast<UCardEffectFragment_Execution_WithParameters>(relatedExecFrag))
			{
				IHoverableContext::Execute_GetHoverContent(HoverObject, Content);
				FFormatNamedArguments Args;
				AsCEF_E_WP->GetFormattingArgs(Args, TheCardIdentity);
				Content = UCardShooterUtilityLibrary::FormatTextForDisplay(Content);
				Content = FText::Format(Content, Args);

				if (IHoverableContext* HoverableContextObject = Cast<IHoverableContext>(HoverObject))
				{
					FFormatNamedArguments Args2;
					HoverableContextObject->GetFormattingArgs(AsCEF_E_WP, Args2);
					Content = FText::Format(Content, Args2);
				}
			}
		}

		return Content;
	}

	FText BuildTooltipTitle() const
	{
		FText Title = FText::FromString(TEXT("Couldn't parse context path."));

		if (!HoverObject)
		{
			return Title;
		}

		if (HoverObject->GetClass()->ImplementsInterface(UHoverableContext::StaticClass()))
		{
			IHoverableContext::Execute_GetHoverContentTitle(HoverObject, Title);
		}

		if (UCardEffectFragment_Base* AsCEF_B = Cast<UCardEffectFragment_Base>(HoverObject))
		{
			return AsCEF_B->GetLocalizableName(CardIdentity.Get());
		}


		return Title;
	}

	void PushDataIntoTooltipWidget(UUserWidget* Widget, const FText& Title, const FText& Content)
	{
		if (!Widget)
		{
			return;
		}

		if (FTextProperty* ContentProp =
			FindFProperty<FTextProperty>(Widget->GetClass(), TEXT("Content")))
		{
			ContentProp->SetPropertyValue_InContainer(Widget, Content);
		}

		if (FTextProperty* TitleProp =
			FindFProperty<FTextProperty>(Widget->GetClass(), TEXT("Title")))
		{
			TitleProp->SetPropertyValue_InContainer(Widget, Title);
		}

		if (FObjectProperty* HoverObjectProp =
			FindFProperty<FObjectProperty>(Widget->GetClass(), TEXT("HoverObject")))
		{
			HoverObjectProp->SetObjectPropertyValue_InContainer(Widget, HoverObject);
		}

		if (FObjectProperty* RelatedProp =
			FindFProperty<FObjectProperty>(Widget->GetClass(), TEXT("relatedExecFrag")))
		{
			RelatedProp->SetObjectPropertyValue_InContainer(Widget, relatedExecFrag);
		}

		if (FStructProperty* CardIdentityProp =
			FindFProperty<FStructProperty>(Widget->GetClass(), TEXT("CardIdentity")))
		{
			if (CardIdentityProp->Struct == TBaseStructure<FCardIdentity>::Get())
			{
				CardIdentityProp->CopyCompleteValue(
					CardIdentityProp->ContainerPtrToValuePtr<void>(Widget),
					&CardIdentity.Get()
				);
			}
		}
	}

	bool HasLiveTooltip() const
	{
		return TooltipUserWidget.IsValid()
			&& TooltipUserWidget->IsInViewport();
	}

	void ClearDeadTooltipReference()
	{
		if (TooltipUserWidget.IsValid() && !TooltipUserWidget->IsInViewport())
		{
			TooltipUserWidget.Reset();
			bTooltipFrozen = false;
		}
		else if (!TooltipUserWidget.IsValid())
		{
			bTooltipFrozen = false;
		}
	}

	void EnsureTooltipCreated(const FGeometry& MyGeometry, const FVector2D& ScreenPosition)
	{
		ClearDeadTooltipReference();

		if (HasLiveTooltip())
		{
			return;
		}

		if (!TooltipWidgetClass || !World.IsValid())
		{
			return;
		}

		APlayerController* PC = World->GetFirstPlayerController();
		if (!PC)
		{
			return;
		}

		UUserWidget* Widget = CreateWidget<UUserWidget>(PC, TooltipWidgetClass);
		if (!Widget)
		{
			return;
		}

		TooltipUserWidget = Widget;

		const FText Content = BuildTooltipContent();
		const FText Title = BuildTooltipTitle();
		PushDataIntoTooltipWidget(Widget, Title, Content);

		Widget->AddToPlayerScreen(9999);

		UpdateTooltipPosition(MyGeometry, ScreenPosition);
	}

	void UpdateTooltipPosition(const FGeometry& MyGeometry, const FVector2D& ScreenPosition)
	{
		ClearDeadTooltipReference();

		if (!HasLiveTooltip() || !World.IsValid())
		{
			return;
		}

		FVector2D PixelPosition;
		FVector2D ViewportPosition;

		USlateBlueprintLibrary::AbsoluteToViewport(World.Get(), ScreenPosition, PixelPosition, ViewportPosition);

		const FVector2D CursorOffset(25.f, 25.f);

		// Desired tooltip position
		FVector2D TooltipPosition = ViewportPosition + CursorOffset;

		// Viewport size
		FVector2D ViewportSize;
		if (GEngine && GEngine->GameViewport)
		{
			GEngine->GameViewport->GetViewportSize(ViewportSize);
		}

		// Tooltip size
		const FVector2D TooltipSize = TooltipUserWidget->GetDesiredSize();

		// If tooltip would go off the right edge, place it to the left of the cursor
		if (TooltipPosition.X + TooltipSize.X > ViewportSize.X)
		{
			TooltipPosition.X = ViewportPosition.X - TooltipSize.X - CursorOffset.X;
		}

		// If tooltip would go off the bottom edge, place it above the cursor
		if (TooltipPosition.Y + TooltipSize.Y > ViewportSize.Y)
		{
			TooltipPosition.Y = ViewportPosition.Y - TooltipSize.Y - CursorOffset.Y;
		}

		// Safety clamp in case tooltip is larger than available space
		TooltipPosition.X = FMath::Clamp(TooltipPosition.X, 0.f, FMath::Max(0.f, ViewportSize.X - TooltipSize.X));
		TooltipPosition.Y = FMath::Clamp(TooltipPosition.Y, 0.f, FMath::Max(0.f, ViewportSize.Y - TooltipSize.Y));
		TooltipUserWidget->SetPositionInViewport(TooltipPosition, false);
	}

	void RemoveTooltip()
	{
		if (TooltipUserWidget.IsValid())
		{
			TooltipUserWidget->RemoveFromParent();
			TooltipUserWidget.Reset();
		}

		bTooltipFrozen = false;
	}

private:
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		EnsureTooltipCreated(
			MyGeometry,
			MouseEvent.GetScreenSpacePosition());
	}

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override
	{
		ClearDeadTooltipReference();

		if (!bTooltipFrozen)
		{
			RemoveTooltip();
		}
	}

	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		ClearDeadTooltipReference();

		if (!bTooltipFrozen)
		{
			EnsureTooltipCreated(
				MyGeometry,
				MouseEvent.GetScreenSpacePosition());

			UpdateTooltipPosition(
				MyGeometry,
				MouseEvent.GetScreenSpacePosition());
		}

		return FReply::Unhandled();
	}

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
		{
			return FReply::Unhandled();
		}

		ClearDeadTooltipReference();

		if (bTooltipFrozen)
		{
			RemoveTooltip();
			return FReply::Handled();
		}

		EnsureTooltipCreated(
			MyGeometry,
			MouseEvent.GetScreenSpacePosition());

		if (HasLiveTooltip())
		{
			bTooltipFrozen = true;
		}

		return FReply::Handled();
	}
};

class FHoverObjectTextDecorator : public FRichTextDecorator
{
private:
	TSubclassOf<UUserWidget> TooltipWidgetClass;
	TWeakObjectPtr<UWorld> World;
	FTextBlockStyle DefaultTextStyle;
	TObjectPtr<UDataTable> TextStyleSet = nullptr;

public:
	FHoverObjectTextDecorator(
		URichTextBlock* InOwner,
		TSubclassOf<UUserWidget> InTooltipWidgetClass,
		FTextBlockStyle InDefaultTextStyle,
		UDataTable* InTextStyleSet)
		: FRichTextDecorator(InOwner)
		, TooltipWidgetClass(InTooltipWidgetClass)
		, World(InOwner ? InOwner->GetWorld() : nullptr)
		, DefaultTextStyle(InDefaultTextStyle)
		, TextStyleSet(InTextStyleSet)
	{
	}

	virtual bool Supports(
		const FTextRunParseResults& Run,
		const FString& Text) const override
	{
		return Run.Name == TEXT("hover")
			&& Run.MetaData.Contains(TEXT("id"));
	}

	virtual TSharedPtr<SWidget> CreateDecoratorWidget(
		const FTextRunInfo& RunInfo,
		const FTextBlockStyle& TextStyle) const override
	{
		const FString* HoveredObjectPath =
			RunInfo.MetaData.Find(TEXT("id"));

		const FString* EffectObjectPath =
			RunInfo.MetaData.Find(TEXT("relatedExec"));

		const FString* CardIdentityHandleString =
			RunInfo.MetaData.Find(TEXT("cardidentityhandle"));

		const FString* Style =
			RunInfo.MetaData.Find(TEXT("style"));

		if (!HoveredObjectPath ||
			HoveredObjectPath->IsEmpty() ||
			!ensure(Style))
		{
			return SNew(STextBlock)
				.Text(FText::GetEmpty());
		}

		UObject* Obj =
			LoadObject<UObject>(nullptr, **HoveredObjectPath);

		UObject* Eff = nullptr;

		if (EffectObjectPath)
		{
			FString OuterPath;
			FString SubObjectName;

			EffectObjectPath->Split(
				TEXT(":"),
				&OuterPath,
				&SubObjectName);

			UObject* Outer =
				LoadObject<UObject>(nullptr, *OuterPath);

			if (Outer)
			{
				Eff = FindObject<UObject>(Outer, *SubObjectName);
			}
		}

		FTextBlockStyle ResolvedTextStyle = DefaultTextStyle;

		if (const FString* StyleString = RunInfo.MetaData.Find(TEXT("style")))
		{
			if (TextStyleSet && !StyleString->IsEmpty())
			{
				const FName StyleRowName(**StyleString);

				if (const FRichTextStyleRow* StyleRow =
					TextStyleSet->FindRow<FRichTextStyleRow>(StyleRowName, TEXT("HoverDecoratorStyle")))
				{
					ResolvedTextStyle = StyleRow->TextStyle;
				}
			}
		}


		if (const FString* StyleString = RunInfo.MetaData.Find(TEXT("style")))
		{
			if (TextStyleSet && !StyleString->IsEmpty())
			{
				const FName StyleRowName(**StyleString);

				if (const FRichTextStyleRow* StyleRow =
					TextStyleSet->FindRow<FRichTextStyleRow>(StyleRowName, TEXT("HoverDecoratorStyle")))
				{
					ResolvedTextStyle = StyleRow->TextStyle;
				}
			}
		}

		FCardIdentity CardIdentity;

		if (CardIdentityHandleString)
		{
			const int32 Index =
				FCString::Atoi(**CardIdentityHandleString);

			ensure(
				UCardIdentityHandleLibrary::GetCachedCardIdentityFromIndex(
					Index,
					CardIdentity));
		}

		return SNew(SHoverObjectWidget)
			.HoverObject(Obj)
			.relatedExecFrag(Eff)
			.TextStyle(ResolvedTextStyle)
			.Text(RunInfo.Content)
			.CardIdentity(CardIdentity)
			.TooltipWidgetClass(TooltipWidgetClass)
			.World(World);
	}
};

TSharedPtr<ITextDecorator> URichTextBlockHoverObjectDecorator::CreateDecorator(
	URichTextBlock* InOwner)
{
	return MakeShareable(
		new FHoverObjectTextDecorator(
			InOwner,
			TooltipWidgetClass,
			DefaultTextStyle,
			TextStyleSet));
}


void UHoverContextObject_Base::GetFormattingArgs(const UCardEffectFragment_Execution_WithParameters* RelatedExec, FFormatNamedArguments& FormattingArguments) const
{
	UCardEffectFragment_Execution_WithParameters::AppendFormattingArgsFromHoverContextObjects(RelatedExec, HoverContextObjects, FormattingArguments);
}

