#include "CardData.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectAbilitySystemComponent.h"
#include "Templates/SharedPointer.h"
#include "Fragments/CardAttributeFragment_Base.h"
#include "Fragments/CardAttributeFragment_Execution.h"
#include "CardIdentity.h"
#include "Util/CardShooterUtility.h"
#include "SaveGame/CardShooterSaveGameSubsystem.h"
#include "SaveGame/CardShooterSaveData.h"

#define LOCTEXT_NAMESPACE "CardData"


void UDedicationData::GetHoverContentTitle_Implementation(FText& OutText) const
{
	OutText = DedicationTitle;
}

void UDedicationData::GetHoverContent_Implementation(FText& OutText) const
{
	OutText = DedicationText;
}

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

void UCardData::GetDedications(FText& OutText) const
{
	TArray<FText> DedicationParts;

	for (UDedicationData* Data : Dedications)
	{
		if (!ensure(Data))
		{
			continue;
		}

		DedicationParts.Add(FText::Format(
			FText::FromString(TEXT("<hover id=\"{0}\" style=\"HoverDedication\">{1}</>")),
			FText::FromString(Data->GetPathName()),
			Data->DedicatedTo));
	}

	OutText = FText::Join(FText::FromString(", "), DedicationParts);
}

FText UCardData::GetAttributesDescription(const FCardIdentity& CardIdentity)
{
	// Gather Attribute Descriptions
	TArray<FText> AttributeDescs;

	for (UCardEffectFragment_Attribute* Attr : CardAttributes)
	{
		if (IsValid(Attr) && Attr->GetShouldShowInDescription())
		{
			FText AttrName = Attr->GetLocalizableName(CardIdentity);
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
		for (const UCardParameter* Param : AsCEF_E_WP->Parameters)
		{
			if (!Param)
			{
				continue;
			}

			OutParameters.AddUnique(Param->ParameterTag);
		}
	}

	if (UCardEffectFragment_Execution_Compound* AsCEF_E_C = Cast<UCardEffectFragment_Execution_Compound>(Exec))
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
		for (const UCardParameter* Param : AsCEF_E_WP->Parameters)
		{
			if (!Param)
			{
				continue;
			}

			OutParameters.AddUnique(Param->MetaData.ScalingGroup);
		}
	}
	
	if (UCardEffectFragment_Execution_Compound* AsCEF_E_C = Cast<UCardEffectFragment_Execution_Compound>(Exec))
	{
		for (UCardEffectFragment_Execution* CompExec : AsCEF_E_C->Executions)
		{
			GatherScalingGroupsFromExecution(CompExec, OutParameters);
		}
	}
}

void UCardData::GetAllUsedParameters(TArray<FGameplayTag>& OutParameters) const
{
	if (ensure(PrimaryExecution))
	{
		GatherParametersFromExecution(PrimaryExecution, OutParameters);
	}
	if (SecondaryExecution)
	{
		GatherParametersFromExecution(SecondaryExecution, OutParameters);
	}

	for (UCardEffectFragment_Attribute* Attribute : CardAttributes)
	{
		for (UCardParameter* Param : Attribute->Parameters)
		{
			if (!Param)
			{
				continue;
			}

			OutParameters.AddUnique(Param->ParameterTag);
		}
	}
}

void UCardData::GetAllUsedScalingGroups(TArray<FGameplayTag>& OutScalingGroups) const
{
	if (ensure(PrimaryExecution))
	{
		GatherScalingGroupsFromExecution(PrimaryExecution, OutScalingGroups);
	}
	if (SecondaryExecution)
	{
		GatherScalingGroupsFromExecution(SecondaryExecution, OutScalingGroups);
	}

	for (UCardEffectFragment_Attribute* Attribute : CardAttributes)
	{
		for (UCardParameter* Param : Attribute->Parameters)
		{
			if (!Param)
			{
				continue;
			}

			OutScalingGroups.AddUnique(Param->MetaData.ScalingGroup);
		}
	}
}

void UCardData::GetAugments(const FCardIdentity& CardIdentity, TArray<UItem_CardAugment*>& OutAugments)
{
	OutAugments.Empty();

	int32 i = 0;
	for (const FGuid& Guid : CardIdentity.Augments)
	{
		OutAugments.Add(UCardData::GetAugmentFromGuid(Guid));
	}
}

UItem_CardAugment* UCardData::GetAugmentFromGuid(FGuid Guid)
{
	for (TObjectIterator<UItem_CardAugment> It; It; ++It)
	{
		UItem_CardAugment* Augment = *It;

		if (!IsValid(Augment) || Augment->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
		{
			continue;
		}

		if (Guid == Augment->ItemGuid)
		{
			return Augment;
		}
	}
	return nullptr;
}

void UCardData::SetAugmentGUIDAtIndex(const FCardIdentityHandle& CardHandle, APlayerController* PC, UItem_CardAugment* CardAugment, int32 Index)
{
	if (!ensure(PC))
	{
		return;
	}

	UCardShooterSaveGameSubsystem* CSSS = PC->GetLocalPlayer()->GetSubsystem<UCardShooterSaveGameSubsystem>();

	if (ensure(CSSS))
	{
		FCardIdentity CardIdentity;
		if (ensure(UCardIdentityHandleLibrary::GetCardIdentityFromHandle(CardHandle, CardIdentity, PC))) {

			UCardShooterSaveData* SaveData = CSSS->GetLocalPlayerSaveData();

			if (CardIdentity.Augments.Num() <= Index)
			{
				CardIdentity.Augments.SetNum(Index + 1);
			}

			//Before assigning the new augment to the card guids, we need to remove the previous ones reference
			FGuid PreviousAugmentGuid = CardIdentity.Augments[Index];
			if (PreviousAugmentGuid.IsValid())			
			{
				UItem_CardAugment* PrevCardAugment = UCardData::GetAugmentFromGuid(PreviousAugmentGuid);
				if (PrevCardAugment)
				{
					PrevCardAugment->UsedByCard = FCardIdentityHandle::Invalid;
				}
			}

			if (CardAugment)
			{
				CardIdentity.Augments[Index] = CardAugment->ItemGuid;
				CardAugment->UsedByCard = CardHandle;
			}
			else
			{
				CardIdentity.Augments[Index] = FGuid(0, 0, 0, 0);
			}

			SaveData->ModifyCardInCollection(PC, CardHandle, CardIdentity);
		}
	}
}

#undef LOCTEXT_NAMESPACE


