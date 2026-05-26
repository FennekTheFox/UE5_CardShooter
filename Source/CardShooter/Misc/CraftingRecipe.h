#pragma once

#include "CraftingRecipe.generated.h"

class UItem;

UCLASS(BlueprintType)
class UCraftingRecipe : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool CanCraft(const TArray<UItem*>& Materials) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	UItem* Craft(const TArray<UItem*>& Materials, UObject* Outer);

	UFUNCTION(BlueprintPure)
	static UCraftingRecipe* GetRecipeCDO(TSubclassOf<UCraftingRecipe> RecipeClass);
};