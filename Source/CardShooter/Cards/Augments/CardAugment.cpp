#include "CardAugment.h"

void UCardAugment::GetAugmentStyleData(const UObject* WorldContextObject, const FCardAugmentData& Data, FSlateBrush& OutBrush)
{
	UClass* LoadedClass = Data.AugmentClass.TryLoadClass<UCardAugment>();
	if (!LoadedClass)
	{
		return;
	}

	const UCardAugment* CDO = LoadedClass->GetDefaultObject<UCardAugment>();

	if (CDO->AugmentBushGenerator)
	{
		CDO->AugmentBushGenerator->GenerateBrush(WorldContextObject, Data, OutBrush);
	}
}

FText UCardAugment::GetAugmentName(const FCardAugmentData& Data)
{
	UClass* LoadedClass = Data.AugmentClass.TryLoadClass<UCardAugment>();
	if (!ensure(LoadedClass))
	{
		return FText::GetEmpty();
	}

	UCardAugment* CDO = LoadedClass->GetDefaultObject<UCardAugment>();

	if (ensure(CDO->AugmentNameGenerator))
	{
		return CDO->AugmentNameGenerator->GenerateText(Data);
	}

	return FText::GetEmpty();
}

FText UCardAugment::GetAugmentDescription(const FCardAugmentData& Data)
{
	UClass* LoadedClass = Data.AugmentClass.TryLoadClass<UCardAugment>();
	if (!ensure(LoadedClass))
	{
		return FText::GetEmpty();
	}

	UCardAugment* CDO = LoadedClass->GetDefaultObject<UCardAugment>();

	if (ensure(CDO->AugmentDescriptionGenerator))
	{
		return CDO->AugmentDescriptionGenerator->GenerateText(Data);
	}

	return FText::GetEmpty();
}

void UCardAugment::GetShardStyleData(const UObject* WorldContextObject, const FCardAugmentShard& Data, FSlateBrush& OutBrush)
{
	UClass* LoadedClass = Data.AugmentType.TryLoadClass<UCardAugment>();
	if (!LoadedClass)
	{
		return;
	}

	const UCardAugment* CDO = LoadedClass->GetDefaultObject<UCardAugment>();

	if (CDO->AugmentBushGenerator)
	{
		CDO->AugmentShardBrushGenerator->GenerateBrush(WorldContextObject, Data, OutBrush);
	}
}

FText UCardAugment::GetShardName(const FCardAugmentShard& Data)
{
	UClass* LoadedClass = Data.AugmentType.TryLoadClass<UCardAugment>();
	if (!ensure(LoadedClass))
	{
		return FText::GetEmpty();
	}


	UCardAugment* CDO = LoadedClass->GetDefaultObject<UCardAugment>();

	if (ensure(CDO->ShardNameGenerator))
	{
		return CDO->ShardNameGenerator->Generate(Data);
	}

	return FText::GetEmpty();
}

FText UCardAugment::GetShardDescription(const FCardAugmentShard& Data)
{
	UClass* LoadedClass = Data.AugmentType.TryLoadClass<UCardAugment>();
	if (!ensure(LoadedClass))
	{
		return FText::GetEmpty();
	}


	UCardAugment* CDO = LoadedClass->GetDefaultObject<UCardAugment>();

	if (ensure(CDO->ShardDescriptionGenerator))
	{
		return CDO->ShardDescriptionGenerator->Generate(Data);
	}

	return FText::GetEmpty();
}

void UItem_CardAugment::GetItemBrush_Implementation(const UObject* WorldContextObject, FSlateBrush& OutBrush)
{
	UCardAugment::GetAugmentStyleData(WorldContextObject, AugmentData, OutBrush);
}

bool UItem_CardAugment::SameItem_Implementation(const UItem* Other) const
{
	if (const UItem_CardAugment* AsAugment = Cast<UItem_CardAugment>(Other))
	{
		return AugmentData == AsAugment->AugmentData;
	}

	return false;
}

FText UItem_CardAugment::GetItemName_Implementation()
{
	return UCardAugment::GetAugmentName(AugmentData);
}

FText UItem_CardAugment::GetItemDescription_Implementation()
{
	return UCardAugment::GetAugmentDescription(AugmentData);
}

bool UItem_CardAugment::SortAmongSames(const UItem* Other) const
{
	const UItem_CardAugment* OtherAsAugment = Cast<UItem_CardAugment>(Other);

	if (ensure(OtherAsAugment))
	{
		if (AugmentData != OtherAsAugment->AugmentData)
		{
			return AugmentData < OtherAsAugment->AugmentData;
		}

		return Super::SortAmongSames(Other);
	}

	return false;
}
