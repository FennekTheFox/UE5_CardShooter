#pragma once

#include "CraftingRecipe.h"
#include "GameplayTagContainer.h"
#include "Math/Range.h"

#include "AugmentCraftingRecipes.generated.h"

class UItem;


UCLASS(BlueprintType, Blueprintable)
class UAugmentRecipe_ParameterModifier : public UCraftingRecipe
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UCurveFloat> DistributionCurve;

	bool CanCraft_Implementation(const TArray<UItem*>& Materials) const override;
	UItem* Craft_Implementation(const TArray<UItem*>& Materials, UObject* Outer) override;
};