#include "Validator_CardDefinesAllParametersUsedByReferences.h"

#include "AssetRegistry/AssetIdentifier.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Cards/CardData.h"
#include "GameplayTagContainer.h"
#include "UObject/UnrealType.h"

DEFINE_LOG_CATEGORY_STATIC(LogCardParameterValidator, Log, All);

bool UValidator_CardDefinesAllParametersUsedByReferences::CanValidateAsset_Implementation(
	const FAssetData& InAssetData,
	UObject* InObject,
	FDataValidationContext& InContext) const
{
	return InObject && InObject->IsA<UCardData>();
}

EDataValidationResult UValidator_CardDefinesAllParametersUsedByReferences::ValidateLoadedAsset_Implementation(
	const FAssetData& InAssetData,
	UObject* InAsset,
	FDataValidationContext& InContext)
{
	UCardData* CardData = Cast<UCardData>(InAsset);
	if (!CardData)
	{
		return EDataValidationResult::NotValidated;
	}

	TArray<FGameplayTag> UsedParametersArray;
	CardData->GetAllUsedParameterTags(UsedParametersArray);
	UsedParametersArray.RemoveAll([this](const FGameplayTag& Tag)
		{
			return Exceptions.Contains(Tag);
		});

	TSet<FGameplayTag> DeclaredParameters;
	DeclaredParameters.Append(UsedParametersArray);

	TSet<FName> VisitedPackages;
	TArray<FAssetData> ReferencedAssets;

	CollectRecursiveDependencies(
		InAssetData.PackageName,
		VisitedPackages,
		ReferencedAssets);

	TMap<FGameplayTag, TArray<FAssetData>> TagToReferencingAssets;

	for (const FAssetData& AssetData : ReferencedAssets)
	{
		TSet<FGameplayTag> TagsFoundInThisAsset;

		CollectParameterTagsFromSearchableNames(
			AssetData.PackageName,
			TagsFoundInThisAsset);

		//UObject* ReferencedAsset = AssetData.GetAsset();
		//if (ReferencedAsset)
		//{
		//	CollectParameterTagsFromObject(
		//		ReferencedAsset,
		//		TagsFoundInThisAsset);
		//}

		for (const FGameplayTag& Tag : TagsFoundInThisAsset)
		{
			TagToReferencingAssets.FindOrAdd(Tag).Add(AssetData);
		}
	}

	bool bIsValid = true;

	for (const TPair<FGameplayTag, TArray<FAssetData>>& Pair : TagToReferencingAssets)
	{
		const FGameplayTag& ParameterTag = Pair.Key;

		if (DeclaredParameters.Contains(ParameterTag))
		{
			continue;
		}

		bIsValid = false;

		FString ReferencingAssetNames;

		for (const FAssetData& AssetData : Pair.Value)
		{
			if (!ReferencingAssetNames.IsEmpty())
			{
				ReferencingAssetNames += TEXT(", ");
			}

			ReferencingAssetNames += AssetData.GetObjectPathString();
		}

		AssetFails(
			InAsset,
			FText::Format(
				NSLOCTEXT(
					"CardValidation",
					"MissingReferencedParameterWithAssetNames",
					"Referenced asset(s) use parameter '{0}', but it is not returned by GetAllUsedParameters(). Referencing asset(s): {1}"),
				FText::FromName(ParameterTag.GetTagName()),
				FText::FromString(ReferencingAssetNames)));
	}

	return bIsValid
		? EDataValidationResult::Valid
		: EDataValidationResult::Invalid;
}


UValidator_CardDefinesAllParametersUsedByReferences::UValidator_CardDefinesAllParametersUsedByReferences()
{
}

void UValidator_CardDefinesAllParametersUsedByReferences::CollectRecursiveDependencies(
	const FName& RootPackageName,
	TSet<FName>& VisitedPackages,
	TArray<FAssetData>& OutAssets)
{
	if (VisitedPackages.Contains(RootPackageName))
	{
		return;
	}

	VisitedPackages.Add(RootPackageName);

	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FName> Dependencies;

	AssetRegistry.GetDependencies(
		RootPackageName,
		Dependencies,
		UE::AssetRegistry::EDependencyCategory::Package);

	for (const FName& DependencyPackage : Dependencies)
	{
		if (VisitedPackages.Contains(DependencyPackage))
		{
			continue;
		}

		TArray<FAssetData> PackageAssets;

		AssetRegistry.GetAssetsByPackageName(
			DependencyPackage,
			PackageAssets);

		OutAssets.Append(PackageAssets);

		CollectRecursiveDependencies(
			DependencyPackage,
			VisitedPackages,
			OutAssets);
	}
}

void UValidator_CardDefinesAllParametersUsedByReferences::CollectParameterTagsFromSearchableNames(
	const FName& PackageName,
	TSet<FGameplayTag>& OutTags)
{
	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	const FAssetIdentifier SourceIdentifier(PackageName);

	TArray<FAssetIdentifier> Dependencies;

	AssetRegistry.GetDependencies(
		SourceIdentifier,
		Dependencies,
		UE::AssetRegistry::EDependencyCategory::SearchableName);

	for (const FAssetIdentifier& Dependency : Dependencies)
	{
		const FName ValueName = Dependency.ValueName;

		if (ValueName.IsNone())
		{
			continue;
		}

		const FString ValueString = ValueName.ToString();

		if (!ValueString.StartsWith(TEXT("Gameplay.Effect.Parameters.")))
		{
			continue;
		}

		UE_LOG(
			LogCardParameterValidator,
			Display,
			TEXT("Parameter searchable-name dependency found. Package: %s Dependency: %s ValueName: %s"),
			*PackageName.ToString(),
			*Dependency.ToString(),
			*ValueName.ToString());

		const FGameplayTag Tag =
			FGameplayTag::RequestGameplayTag(ValueName, false);

		if (Tag.IsValid())
		{
			OutTags.Add(Tag);
		}
		else
		{
			UE_LOG(
				LogCardParameterValidator,
				Display,
				TEXT("Searchable-name dependency looked like a parameter tag but is not a valid registered GameplayTag. Package: %s ValueName: %s"),
				*PackageName.ToString(),
				*ValueName.ToString());
		}
	}
}

bool UValidator_CardDefinesAllParametersUsedByReferences::IsParameterTag(
	const FGameplayTag& Tag)
{
	if (!Tag.IsValid())
	{
		return false;
	}

	return Tag.ToString().StartsWith(TEXT("Gameplay.Effect.Parameters."));
}

void UValidator_CardDefinesAllParametersUsedByReferences::CollectParameterTagsFromObject(
	UObject* Object,
	TSet<FGameplayTag>& OutTags)
{
	if (!Object)
	{
		return;
	}

	CollectParameterTagsFromStruct(
		Object,
		Object->GetClass(),
		OutTags);
}

void UValidator_CardDefinesAllParametersUsedByReferences::CollectParameterTagsFromStruct(
	void* StructValue,
	const UStruct* StructType,
	TSet<FGameplayTag>& OutTags)
{
	if (!StructValue || !StructType)
	{
		return;
	}

	for (TFieldIterator<FProperty> PropertyIt(StructType); PropertyIt; ++PropertyIt)
	{
		FProperty* Property = *PropertyIt;
		if (!Property)
		{
			continue;
		}

		void* ValuePtr = Property->ContainerPtrToValuePtr<void>(StructValue);

		if (FStructProperty* StructProperty = CastField<FStructProperty>(Property))
		{
			if (StructProperty->Struct == FGameplayTag::StaticStruct())
			{
				const FGameplayTag* Tag = static_cast<FGameplayTag*>(ValuePtr);

				if (Tag && IsParameterTag(*Tag))
				{
					UE_LOG(
						LogCardParameterValidator,
						Display,
						TEXT("Parameter property dependency found. Struct: %s Property: %s Tag: %s"),
						*StructType->GetName(),
						*Property->GetName(),
						*Tag->ToString());

					OutTags.Add(*Tag);
				}
			}
			else if (StructProperty->Struct == FGameplayTagContainer::StaticStruct())
			{
				const FGameplayTagContainer* Container =
					static_cast<FGameplayTagContainer*>(ValuePtr);

				if (Container)
				{
					for (const FGameplayTag& Tag : *Container)
					{
						if (IsParameterTag(Tag))
						{
							UE_LOG(
								LogCardParameterValidator,
								Display,
								TEXT("Parameter container property dependency found. Struct: %s Property: %s Tag: %s"),
								*StructType->GetName(),
								*Property->GetName(),
								*Tag.ToString());

							OutTags.Add(Tag);
						}
					}
				}
			}
			else
			{
				CollectParameterTagsFromStruct(
					ValuePtr,
					StructProperty->Struct,
					OutTags);
			}
		}
		else if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
		{
			FScriptArrayHelper Helper(ArrayProperty, ValuePtr);

			for (int32 Index = 0; Index < Helper.Num(); ++Index)
			{
				FStructProperty* InnerStruct =
					CastField<FStructProperty>(ArrayProperty->Inner);

				if (!InnerStruct)
				{
					continue;
				}

				void* ElementPtr = Helper.GetRawPtr(Index);

				if (InnerStruct->Struct == FGameplayTag::StaticStruct())
				{
					const FGameplayTag* Tag = static_cast<FGameplayTag*>(ElementPtr);

					if (Tag && IsParameterTag(*Tag))
					{
						UE_LOG(
							LogCardParameterValidator,
							Display,
							TEXT("Parameter array property dependency found. Struct: %s Property: %s Index: %d Tag: %s"),
							*StructType->GetName(),
							*Property->GetName(),
							Index,
							*Tag->ToString());

						OutTags.Add(*Tag);
					}
				}
				else if (InnerStruct->Struct == FGameplayTagContainer::StaticStruct())
				{
					const FGameplayTagContainer* Container =
						static_cast<FGameplayTagContainer*>(ElementPtr);

					if (Container)
					{
						for (const FGameplayTag& Tag : *Container)
						{
							if (IsParameterTag(Tag))
							{
								UE_LOG(
									LogCardParameterValidator,
									Display,
									TEXT("Parameter array container property dependency found. Struct: %s Property: %s Index: %d Tag: %s"),
									*StructType->GetName(),
									*Property->GetName(),
									Index,
									*Tag.ToString());

								OutTags.Add(Tag);
							}
						}
					}
				}
				else
				{
					CollectParameterTagsFromStruct(
						ElementPtr,
						InnerStruct->Struct,
						OutTags);
				}
			}
		}
	}
}