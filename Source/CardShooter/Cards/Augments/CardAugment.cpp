#include "CardAugment.h"

void UCardAugment::GetAugmentStyleData(const FCardAugmentData& Data, class UTexture2D*& Texture, FLinearColor& Tint)
{
	Texture = DefaultAugmentTexutre;
	Tint = DefaultTint;
}

