// Validator_CardDescUsesAllParameters.h

#pragma once

#include "CoreMinimal.h"
#include "EditorValidatorBase.h"
#include "Validator_CardDescUsesAllParameters.generated.h"

UCLASS()
class UValidator_CardDescUsesAllParameters : public UEditorValidatorBase
{
	GENERATED_BODY()

public:
	virtual bool CanValidateAsset_Implementation(
		const FAssetData& InAssetData,
		UObject* InAsset,
		FDataValidationContext& Context) const override;

	EDataValidationResult ValidateLoadedAsset_Implementation(
		const FAssetData& InAssetData, 
		UObject* InAsset, 
		FDataValidationContext& Context) override;
};