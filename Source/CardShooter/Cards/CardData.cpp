#include "CardData.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectAbilitySystemComponent.h"
#include "Templates/SharedPointer.h"
#include "Fragments/CardAttributeFragment_Base.h"
#include "Fragments/CardAttributeFragment_Execution.h"
#include "CardIdentity.h"
#include "Util/CardShooterUtility.h"

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

FText UCardData::GetDescription(const FCardIdentity& CardIdentity)
{
	return UCardShooterUtilityLibrary::FormatTextForDisplay(GetRawDescription(CardIdentity));
}

FText UCardData::GetRawDescription(const FCardIdentity& CardIdentity)
{
	// Ensure primary execution exists
	if (!ensure(PrimaryExecution))
	{
		return FText::GetEmpty();
	}

	TArray<FText> Sections;

	// Attributes

	// Gather Attribute Descriptions
	FText AttributesDesc = GetAttributesDescription(CardIdentity);

	if (!AttributesDesc.IsEmpty())
	{
		Sections.Add(AttributesDesc);
	}

	Sections.Add(GetExecutionsDescription(CardIdentity));

	// Final join with spacing
	return FText::Join(FText::FromString("\n\n"), Sections);
}

FText UCardData::GetAttributesDescription(const FCardIdentity& CardIdentity)
{
	// Gather Attribute Descriptions
	TArray<FText> AttributeDescs;

	for (UCardEffectFragment_Attribute* Attr : CardAttributes)
	{
		if (IsValid(Attr) && Attr->GetShouldShowInDescription())
		{
			FText AttrName = Attr->GetLocalizableName();
			FText Wrapped = FText::Format(
				FText::FromString(TEXT("<hover id=\"{0}\" cardidentityhandle=\"{1}\" style=\"HoverContent\">{2}</>")),
				FText::FromString(Attr->GetPathName()),
				FText::AsNumber(UCardIdentityHandleLibrary::GetOrAddCardIdentityHandle(CardIdentity).Index),
				AttrName
			);
			AttributeDescs.Add(Wrapped);
		}
	}

	FText AttributesDesc = FText::Join(FText::FromString(", "), AttributeDescs);

	return UCardShooterUtilityLibrary::FormatTextForDisplay(AttributesDesc);
}

FText UCardData::GetExecutionsDescription(const FCardIdentity& CardIdentity)
{
	TArray<FText> Sections;

	FText PrimaryInput = LOCTEXT("PrimaryInput", "<input id=\"/Game/Blueprint/Input/TacticalMode/CardExecution/IA_ExecuteCardPrimary\"/>");
	FText SecondaryInput = LOCTEXT("SecondaryInput", "<input id=\"/Game/Blueprint/Input/TacticalMode/CardExecution/IA_ExecuteCardSecondary\"/>");

	// Primary execution
	if (!SecondaryExecution)
	{
		Sections.Add(
			FText::Format(
				LOCTEXT("NoSecondaryLayout", "{0}|{1}: {2}"),
				PrimaryInput,
				SecondaryInput,
				PrimaryExecution->GetLocalizableDescription(CardIdentity)
			)
		);
	}
	else
	{
		TArray<FText> PrimaryParts;
		PrimaryParts.Add(PrimaryInput);
		PrimaryParts.Add(PrimaryExecution->GetLocalizableDescription(CardIdentity));
		Sections.Add(FText::Join(FText::FromString(": "), PrimaryParts));
	}


	// Secondary execution (optional)
	if (SecondaryExecution)
	{

		TArray<FText> SecondaryParts;
		SecondaryParts.Add(SecondaryInput);
		SecondaryParts.Add(SecondaryExecution->GetLocalizableDescription(CardIdentity));
		Sections.Add(FText::Join(FText::FromString(": "), SecondaryParts));
	}

	// Final join with spacing
	return UCardShooterUtilityLibrary::FormatTextForDisplay((FText::Join(FText::FromString("\n\n"), Sections)));
}

void GatherParametersFromExecution(UCardEffectFragment_Execution* Exec, TArray<FGameplayTag>& OutParameters)
{
	if (UCardEffectFragment_Execution_WithParameters* AsCEF_E_WP = Cast<UCardEffectFragment_Execution_WithParameters>(Exec))
	{
		for (auto&& KVPair : AsCEF_E_WP->SetByCallerMagnitudes)
		{
			OutParameters.AddUnique(KVPair.Key);
		}
	}
	else if (UCardEffectFragment_Execution_Compound* AsCEF_E_C = Cast<UCardEffectFragment_Execution_Compound>(Exec))
	{
		for (UCardEffectFragment_Execution* CompExec : AsCEF_E_C->Executions)
		{
			GatherParametersFromExecution(CompExec, OutParameters);
		}
	}
}


void GatherScalingGroupsFromExecution(UCardEffectFragment_Execution* Exec, TArray<FGameplayTag>& OutParameters)
{
	if (UCardEffectFragment_Execution_WithParameters* AsCEF_E_WP = Cast<UCardEffectFragment_Execution_WithParameters>(Exec))
	{
		for (auto&& KVPair : AsCEF_E_WP->SetByCallerMagnitudes)
		{
			OutParameters.AddUnique(KVPair.Value.ScalingGroup);
		}
	}
	else if (UCardEffectFragment_Execution_Compound* AsCEF_E_C = Cast<UCardEffectFragment_Execution_Compound>(Exec))
	{
		for (UCardEffectFragment_Execution* CompExec : AsCEF_E_C->Executions)
		{
			GatherScalingGroupsFromExecution(CompExec, OutParameters);
		}
	}
}

void UCardData::GetAllUsedParameters(TArray<FGameplayTag>& OutParameters)
{
	if (ensure(PrimaryExecution))
	{
		GatherParametersFromExecution(PrimaryExecution, OutParameters);
	}
	if (SecondaryExecution)
	{
		GatherParametersFromExecution(SecondaryExecution, OutParameters);
	}
}

void UCardData::GetAllUsedScalingGroups(TArray<FGameplayTag>& OutParameters)
{
	if (ensure(PrimaryExecution))
	{
		GatherScalingGroupsFromExecution(PrimaryExecution, OutParameters);
	}
	if (SecondaryExecution)
	{
		GatherScalingGroupsFromExecution(SecondaryExecution, OutParameters);
	}
}

#undef LOCTEXT_NAMESPACE
