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
#include "Fragments/CardEffectFragment_Stable.h"

#define LOCTEXT_NAMESPACE "CardData"


void UDedicationData::GetHoverContentTitle_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const UCardEffectFragment_Execution_WithParameters* RelatedExec) const
{
	OutText = DedicationTitle;
}

void UDedicationData::GetHoverContent_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const FCardIdentity& CardIdentity, const UCardEffectFragment_Execution_WithParameters* RelatedExec) const
{
	OutText = DedicationText;
}

void UCardData::GetIdentity(FCardIdentity& Identity)
{
	Identity.AssetTag = AssetTag;
	Identity.CardDataPath = FSoftObjectPath(this);
}

void UCardData::GetCardAttributes(const FCardIdentity& Identity, TArray<UCardEffectFragment_Attribute*>& OutAttributes) const
{
	OutAttributes = CardAttributes;
}

TArray<UCardEffectFragment_Attribute*> UCardData::GetAllAttributesOfClass(const FCardIdentity& Identity, TSubclassOf<UCardEffectFragment_Attribute> Class)
{
	TArray<UCardEffectFragment_Attribute*> OutArray;
	GetCardAttributes(Identity, OutArray);

	OutArray = OutArray.FilterByPredicate([Class](UCardEffectFragment_Attribute* CardAttribute)
		{
			return CardAttribute->IsA(Class);
		});

	return OutArray;
}

UCardEffectFragment_Attribute* UCardData::GetAttributeOfClass(FCardIdentity& Identity, TSubclassOf<UCardEffectFragment_Attribute> Class)
{
	TArray<UCardEffectFragment_Attribute*> Attributes;
	GetCardAttributes(Identity, Attributes);

	for (UCardEffectFragment_Attribute* Attribute : Attributes)
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

	TArray<UCardEffectFragment_Attribute*> Attributes;
	GetCardAttributes(CardIdentity, Attributes);

	for (UCardEffectFragment_Attribute* Attr : Attributes)
	{
		if (IsValid(Attr) && Attr->GetShouldShowInDescription())
		{
			FText AttrName = Attr->GetLocalizableName(CardIdentity);
			FText Wrapped = FText::Format(
				FText::FromString(TEXT("<hover id=\"{0}\" cardidentityhandle=\"{1}\" style=\"HoverContent\">{2}</>")),
				FText::FromString(Attr->GetPathName()),
				FText::AsNumber(UCardIdentityHandleLibrary::GetOrAddCardCachedIdentityHandle(CardIdentity).Index),
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

void GatherParameterTagsFromExecution(UCardEffectFragment_Execution* Exec, TArray<FGameplayTag>& OutParameters)
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
			GatherParameterTagsFromExecution(CompExec, OutParameters);
		}
	}
}

void GatherLocalizedParameterTagsFromExecution(UCardEffectFragment_Execution* Exec, TArray<FGameplayTag>& OutParameters)
{
	if (UCardEffectFragment_Execution_WithParameters* AsCEF_E_WP = Cast<UCardEffectFragment_Execution_WithParameters>(Exec))
	{
		for (const UCardParameter* Param : AsCEF_E_WP->Parameters)
		{
			if (!Param || !Param->bNeedsLocalization)
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
			GatherParameterTagsFromExecution(CompExec, OutParameters);
		}
	}
}

void GatherParametersFromExecution(UCardEffectFragment_Execution* Exec, TArray<UCardParameter*>& OutParameters)
{
	if (UCardEffectFragment_Execution_WithParameters* AsCEF_E_WP = Cast<UCardEffectFragment_Execution_WithParameters>(Exec))
	{
		for (UCardParameter* Param : AsCEF_E_WP->Parameters)
		{
			if (!Param)
			{
				continue;
			}

			OutParameters.AddUnique(Param);
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

void UCardData::GetAllUsedParameterTags(TArray<FGameplayTag>& OutParameters) const
{

	if (ensure(PrimaryExecution))
	{
		GatherParameterTagsFromExecution(PrimaryExecution, OutParameters);
	}
	if (SecondaryExecution)
	{
		GatherParameterTagsFromExecution(SecondaryExecution, OutParameters);
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

void UCardData::GetAllLocalizedParameterTags(TArray<FGameplayTag>& OutParameters) const
{

	if (ensure(PrimaryExecution))
	{
		GatherLocalizedParameterTagsFromExecution(PrimaryExecution, OutParameters);
	}
	if (SecondaryExecution)
	{
		GatherLocalizedParameterTagsFromExecution(SecondaryExecution, OutParameters);
	}

	for (UCardEffectFragment_Attribute* Attribute : CardAttributes)
	{
		for (UCardParameter* Param : Attribute->Parameters)
		{
			if (!Param || !Param->bNeedsLocalization)
			{
				continue;
			}

			OutParameters.AddUnique(Param->ParameterTag);
		}
	}
}

void UCardData::GetAllUsedParameters(TArray< UCardParameter*>& OutParameters) const
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
			OutParameters.AddUnique(Param);
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

	OutScalingGroups.Remove(FGameplayTag());
}

void GatherParametersFromExecutionByPredicate(
	UCardEffectFragment_Execution* Exec,
	TArray<UCardParameter*>& OutParameters,
	TFunctionRef<bool(const UCardParameter*)> Predicate)
{
	if (!Exec)
	{
		return;
	}

	if (const UCardEffectFragment_Execution_WithParameters* WithParams =
		Cast<UCardEffectFragment_Execution_WithParameters>(Exec))
	{
		for (UCardParameter* Param : WithParams->Parameters)
		{
			if (Param && Predicate(Param))
			{
				OutParameters.AddUnique(Param);
			}
		}
	}

	if (const UCardEffectFragment_Execution_Compound* Compound =
		Cast<UCardEffectFragment_Execution_Compound>(Exec))
	{
		for (UCardEffectFragment_Execution* ChildExec : Compound->Executions)
		{
			GatherParametersFromExecutionByPredicate(
				ChildExec,
				OutParameters,
				Predicate);
		}
	}
}

void UCardData::GetParametersByPredicate(TArray<UCardParameter*>& OutParameters, TFunctionRef<bool(const UCardParameter*)> Predicate) const
{
	if (PrimaryExecution)
	{
		GatherParametersFromExecutionByPredicate(
			PrimaryExecution,
			OutParameters,
			Predicate);
	}

	if (SecondaryExecution)
	{
		GatherParametersFromExecutionByPredicate(
			SecondaryExecution,
			OutParameters,
			Predicate);
	}

	for (UCardEffectFragment_Attribute* Attribute : CardAttributes)
	{
		if (!Attribute)
		{
			continue;
		}

		for (UCardParameter* Param : Attribute->Parameters)
		{
			if (Param && Predicate(Param))
			{
				OutParameters.AddUnique(Param);
			}
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

#if WITH_EDITOR
void UCardData::PostLoad()
{
	Super::PostLoad();
	RecalculateDominantHue();
}

void UCardData::RecalculateDominantHue()
{
	DominantHue = 0.f;

	UTexture2D* Texture = CardTexture.LoadSynchronous();

	if (!Texture)
	{
		return;
	}

	FTextureSource& Source = Texture->Source;

	if (!Source.IsValid())
	{
		return;
	}

	const int32 Width = Source.GetSizeX();
	const int32 Height = Source.GetSizeY();

	if (Width <= 0 || Height <= 0)
	{
		return;
	}

	TArray64<uint8> RawData;

	if (!Source.GetMipData(RawData, 0))
	{
		return;
	}

	const FColor* Pixels =
		reinterpret_cast<const FColor*>(RawData.GetData());

	TArray<int32> HueHistogram;
	HueHistogram.Init(0, 360);

	const int32 PixelCount = Width * Height;

	for (int32 PixelIndex = 0; PixelIndex < PixelCount; ++PixelIndex)
	{
		const FLinearColor LinearColor =
			Pixels[PixelIndex].ReinterpretAsLinear();

		FLinearColor HSV = LinearColor.LinearRGBToHSV();

		const float Hue = HSV.R;
		const float Saturation = HSV.G;
		const float Value = HSV.B;

		// Ignore black pixels
		if (Value < 0.15f)
		{
			continue;
		}

		// Ignore gray pixels
		if (Saturation < 0.15f)
		{
			continue;
		}

		const int32 HueBin =
			FMath::Clamp(
				FMath::RoundToInt(Hue),
				0,
				359);

		++HueHistogram[HueBin];
	}

	int32 BestBin = 0;
	int32 BestCount = 0;

	for (int32 BinIndex = 0; BinIndex < HueHistogram.Num(); ++BinIndex)
	{
		if (HueHistogram[BinIndex] > BestCount)
		{
			BestCount = HueHistogram[BinIndex];
			BestBin = BinIndex;
		}
	}

	DominantHue = static_cast<float>(BestBin);
#if WITH_EDITOR
	Modify();
#endif
}
#endif

#undef LOCTEXT_NAMESPACE


