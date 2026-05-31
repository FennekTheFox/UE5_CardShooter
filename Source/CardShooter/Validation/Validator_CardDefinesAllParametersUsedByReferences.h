#pragma once

#include "CoreMinimal.h"
#include "EditorValidatorBase.h"
#include "GameplayTagContainer.h"
#include "Validator_CardDefinesAllParametersUsedByReferences.generated.h"

UCLASS()
class UValidator_CardDefinesAllParametersUsedByReferences
	: public UEditorValidatorBase
{
	GENERATED_BODY()

	UValidator_CardDefinesAllParametersUsedByReferences();

public:
	UPROPERTY()
	TArray<FGameplayTag> Exceptions;

	virtual bool CanValidateAsset_Implementation(
		const FAssetData& InAssetData,
		UObject* InObject,
		FDataValidationContext& InContext) const override;

	virtual EDataValidationResult ValidateLoadedAsset_Implementation(
		const FAssetData& InAssetData,
		UObject* InAsset,
		FDataValidationContext& InContext) override;

private:

	static void CollectRecursiveDependencies(
		const FName& RootPackageName,
		TSet<FName>& VisitedPackages,
		TArray<FAssetData>& OutAssets);

	static void CollectParameterTagsFromObject(
		UObject* Object,
		TSet<FGameplayTag>& OutTags);

	static void CollectParameterTagsFromStruct(
		void* StructValue,
		const UStruct* StructType,
		TSet<FGameplayTag>& OutTags);

	static void CollectParameterTagsFromSearchableNames(
		const FName& PackageName,
		TSet<FGameplayTag>& OutTags);

	static bool IsParameterTag(const FGameplayTag& Tag);
};