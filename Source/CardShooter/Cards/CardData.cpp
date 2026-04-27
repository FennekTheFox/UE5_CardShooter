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

UTexture2D* UCardData::GetIcon_Implementation()
{
	return CardIcon;
}

FText UCardData::GetDescription_Implementation()
{

	// Gather Attribute Descriptions
	TArray<FText> AttributeDescs;

	for (UCardEffectFragment_Attribute* Attr : CardAttributes)
	{
		if (IsValid(Attr))
		{
			AttributeDescs.Add(Attr->GetDescription());
		}
	}

	FText AttributesDesc = FText::Join(FText::FromString(", "), AttributeDescs);

	// Ensure primary execution exists
	if (!ensure(PrimaryExecution))
	{
		return FText::GetEmpty();
	}

	TArray<FText> Sections;

	// Attributes
	if (!AttributesDesc.IsEmpty())
	{
		Sections.Add(AttributesDesc);
	}

	// Primary execution
	if (SecondaryExecution)
	{
		TArray<FText> PrimaryParts;
		PrimaryParts.Add(LOCTEXT("PrimarySpecifier", "Primary:"));
		PrimaryParts.Add(PrimaryExecution->GetDescription());
		Sections.Add(FText::Join(FText::FromString(" "), PrimaryParts));
	}
	else
	{
		Sections.Add(PrimaryExecution->GetDescription());
	}


	// Secondary execution (optional)
	if (SecondaryExecution)
	{
		TArray<FText> SecondaryParts;
		SecondaryParts.Add(LOCTEXT("SecondarySpecifier", "Secondary:"));
		SecondaryParts.Add(PrimaryExecution->GetDescription());
		Sections.Add(FText::Join(FText::FromString(" "), SecondaryParts));
	}

	// Final join with spacing
	return FText::Join(FText::FromString("\n\n"), Sections);
}

FText UCardData::GetName_Implementation()
{
	return CardName;
}

#undef LOCTEXT_NAMESPACE
