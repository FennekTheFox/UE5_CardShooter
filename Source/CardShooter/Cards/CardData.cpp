#include "CardData.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectAbilitySystemComponent.h"
#include "Templates/SharedPointer.h"
#include "CardAttributeFragment_Base.h"
#include "CardAttributeFragment_Execution.h"

#define LOCTEXT_NAMESPACE "CardData"

void UCardData::GetIdentity(FCardIdentity& Identity)
{
	Identity.AssetTag = AssetTag;
	Identity.CardDataPath = FSoftObjectPath(this);
}

TArray<UCardEffectFragment_Attribute*> UCardData::GetAllAttributesOfClass(TSubclassOf<UCardEffectFragment_Attribute> Class)
{
	TArray<UCardEffectFragment_Attribute*> OutArray = CardAttributes;

	OutArray = OutArray.FilterByPredicate([Class](UCardEffectFragment_Attribute* CardAttribute)
		{
			return CardAttribute->IsA(Class);
		});

	return OutArray;
}

UCardEffectFragment_Attribute* UCardData::GetAttributeOfClass(TSubclassOf<UCardEffectFragment_Attribute> Class)
{
	for (UCardEffectFragment_Attribute* Attribute : CardAttributes)
	{
		if (Attribute->IsA(Class))
		{
			return Attribute;
		}
	}
	return nullptr;
}

FText UCardData::GetDescription()
{
	// Ensure primary execution exists
	if (!ensure(PrimaryExecution))
	{
		return FText::GetEmpty();
	}

	// Gather Attribute Descriptions
	TArray<FText> AttributeDescs;

	for (UCardEffectFragment_Attribute* Attr : CardAttributes)
	{
		if (IsValid(Attr))
		{
			FText AttrName = Attr->GetLocalizableName();
			FText Wrapped = FText::Format(
				FText::FromString(TEXT("<hover id=\"{0}\">{1}</>")),
				FText::FromString(Attr->GetPathName()),
				AttrName
			);
			AttributeDescs.Add(Wrapped);
		}
	}

	FText AttributesDesc = FText::Join(FText::FromString(", "), AttributeDescs);

	TArray<FText> Sections;

	// Attributes
	if (!AttributesDesc.IsEmpty())
	{
		Sections.Add(AttributesDesc);
	}

	FText PrimaryInput = LOCTEXT("PrimaryInput", "<input id=\"/Game/Blueprint/Input/CardExecution/IA_ExecuteCardPrimary\"/>");
	FText SecondaryInput = LOCTEXT("SecondaryInput", "<input id=\"/Game/Blueprint/Input/CardExecution/IA_ExecuteCardSecondary\"/>");

	// Primary execution
	if (!SecondaryExecution)
	{
		Sections.Add(
			FText::Format(
				LOCTEXT("NoSecondaryLayout", "{0}|{1}: {2}"),
				PrimaryInput,
				SecondaryInput,
				PrimaryExecution->GetLocalizableDescription()
			)
		);
	}
	else
	{
		TArray<FText> PrimaryParts;
		PrimaryParts.Add(PrimaryInput);
		PrimaryParts.Add(PrimaryExecution->GetLocalizableDescription());
		Sections.Add(FText::Join(FText::FromString(": "), PrimaryParts));
	}


	// Secondary execution (optional)
	if (SecondaryExecution)
	{

		TArray<FText> SecondaryParts;
		SecondaryParts.Add(SecondaryInput);
		SecondaryParts.Add(SecondaryExecution->GetLocalizableDescription());
		Sections.Add(FText::Join(FText::FromString(": "), SecondaryParts));
	}

	// Final join with spacing
	return FText::Join(FText::FromString("\n\n"), Sections);
}

#undef LOCTEXT_NAMESPACE
