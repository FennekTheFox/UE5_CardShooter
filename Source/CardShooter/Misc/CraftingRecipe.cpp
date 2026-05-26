#include "CraftingRecipe.h"

bool UCraftingRecipe::CanCraft_Implementation(const TArray<UItem*>& Materials) const
{
	return false;
}

UItem* UCraftingRecipe::Craft_Implementation(const  TArray<UItem*>& Materials, UObject* Outer)
{
	return nullptr;
}

UCraftingRecipe* UCraftingRecipe::GetRecipeCDO(TSubclassOf<UCraftingRecipe> RecipeClass)
{
	return Cast<UCraftingRecipe>(RecipeClass->GetDefaultObject());
}

