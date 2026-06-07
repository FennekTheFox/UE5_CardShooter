#include "Validator_CardAllDefinedParametersUsedByReferences.h"

#include "AssetRegistry/AssetIdentifier.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Cards/CardData.h"
#include "GameplayTagContainer.h"
#include "UObject/UnrealType.h"
#include "Cards/Fragments/CardAttributeFragment_Base.h"

namespace
{
	bool IsCardParameterAsset(UObject* Asset)
	{
		if (!Asset)
		{
			return false;
		}

		if (Asset->IsA(UCardParameter::StaticClass()))
		{
			return true;
		}

		if (const UBlueprint* Blueprint = Cast<UBlueprint>(Asset))
		{
			return Blueprint->GeneratedClass &&
				Blueprint->GeneratedClass->IsChildOf(UCardParameter::StaticClass());
		}

		return false;
	}
}

bool UValidator_CardAllDefinedParametersUsedByReferences::CanValidateAsset_Implementation(
	const FAssetData& InAssetData,
	UObject* InObject,
	FDataValidationContext& InContext) const
{
	return InObject && InObject->IsA<UCardData>();
}

EDataValidationResult UValidator_CardAllDefinedParametersUsedByReferences::ValidateLoadedAsset_Implementation(
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
	CardData->GetAllUsedParameters(UsedParametersArray);
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

	TSet<FGameplayTag> ReferencedParameterTags;

	// Important:
	// Do NOT scan the UCardData asset itself.
	// This validator only cares whether declared parameters are used by referenced assets.

	for (const FAssetData& AssetData : ReferencedAssets)
	{
		UObject* ReferencedAsset = AssetData.GetAsset();

		if (IsCardParameterAsset(ReferencedAsset))
		{
			UE_LOG(
				LogTemp,
				Verbose,
				TEXT("Skipping parameter asset: %s"),
				*GetNameSafe(ReferencedAsset));

			continue;
		}

		CollectParameterTagsFromSearchableNames(
			AssetData.PackageName,
			ReferencedParameterTags);

		if (ReferencedAsset)
		{
			CollectParameterTagsFromObject(
				ReferencedAsset,
				ReferencedParameterTags);
		}
	}

	bool bIsValid = true;

	for (const FGameplayTag& DeclaredParameter : DeclaredParameters)
	{
		if (ReferencedParameterTags.Contains(DeclaredParameter))
		{
			continue;
		}

		bIsValid = false;

		AssetFails(
			InAsset,
			FText::Format(
				NSLOCTEXT(
					"CardValidation",
					"DeclaredParameterNotUsedByReferences",
					"GetAllUsedParameters() returns parameter '{0}', but no recursively referenced asset uses it."),
				FText::FromName(DeclaredParameter.GetTagName())));
	}

	return bIsValid
		? EDataValidationResult::Valid
		: EDataValidationResult::Invalid;
}


UValidator_CardAllDefinedParametersUsedByReferences::UValidator_CardAllDefinedParametersUsedByReferences()
{
	// These are exceptions because the only reference to them will be the utility fragment, which isn't a referenced asset
	Exceptions.Add(FGameplayTag::RequestGameplayTag("Gameplay.Effect.Parameters.PrimaryUtilityCooldown"));
	Exceptions.Add(FGameplayTag::RequestGameplayTag("Gameplay.Effect.Parameters.SecondaryUtilityCooldown"));
}

void UValidator_CardAllDefinedParametersUsedByReferences::CollectRecursiveDependencies(const FName& RootPackageName, TSet<FName>& VisitedPackages, TArray<FAssetData>& OutAssets)
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

void UValidator_CardAllDefinedParametersUsedByReferences::CollectParameterTagsFromSearchableNames(
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

		const FGameplayTag Tag =
			FGameplayTag::RequestGameplayTag(ValueName, false);

		if (Tag.IsValid())
		{
			OutTags.Add(Tag);
		}
	}
}

bool UValidator_CardAllDefinedParametersUsedByReferences::IsParameterTag(
	const FGameplayTag& Tag)
{
	if (!Tag.IsValid())
	{
		return false;
	}

	return Tag.ToString().StartsWith(TEXT("Gameplay.Effect.Parameters."));
}

void UValidator_CardAllDefinedParametersUsedByReferences::CollectParameterTagsFromObject(
	UObject* Object,
	TSet<FGameplayTag>& OutTags)
{
	if (!Object)
	{
		return;
	}

	if (Object->IsA(UCardParameter::StaticClass()))
	{
		return;
	}

	CollectParameterTagsFromStruct(
		Object,
		Object->GetClass(),
		OutTags);
}

void UValidator_CardAllDefinedParametersUsedByReferences::CollectParameterTagsFromStruct(
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