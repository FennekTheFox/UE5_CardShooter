#include "CardAugment.h"

void UCardAugment::GetAugmentStyleData(const FCardAugmentData& Data, UTexture2D*& Texture, FLinearColor& Tint)
{
	UClass* LoadedClass = Data.AugmentClass.TryLoadClass<UCardAugment>();
	if (!LoadedClass)
	{
		return;
	}

	const UCardAugment* CDO = LoadedClass->GetDefaultObject<UCardAugment>();

	Texture = CDO->DefaultAugmentTexutre;
	Tint = CDO->DefaultTint;
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
		return CDO->AugmentNameGenerator->GenerateName(Data);
	}

	return FText::GetEmpty();
}

void UCardAugment::GetShardStyleData(const FCardAugmentShard& Data, UTexture2D*& Texture, FLinearColor& Tint)
{
	UClass* LoadedClass = Data.AugmentType.TryLoadClass<UCardAugment>();
	if (!LoadedClass)
	{
		return;
	}

	UCardAugment* CDO = LoadedClass->GetDefaultObject<UCardAugment>();

	if (UTexture2D** FoundValue = CDO->DefaultShardTextures.Find(Data.PieceType))
	{
		Texture = *FoundValue;
	}

	Tint = CDO->DefaultTint;
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

