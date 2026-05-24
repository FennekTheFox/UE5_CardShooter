#include "Item.h"

void UItem::GetItemBrush_Implementation(FSlateBrush& OutBrush)
{

}

bool UItem::SameItem_Implementation(const UItem* Other) const 
{
	return false;
}

FText UItem::GetItemName_Implementation()
{
	return FText::GetEmpty();
}

FText UItem::GetItemDescription_Implementation()
{
	return FText::GetEmpty();
}

int32 UItem_Stackable::AddToCount(int32 Amount)
{
	Count += Amount;
	const int32 Overflow = FMath::Max(0, Count - MaxStackSize);
	Count = FMath::Clamp(Count, 0, MaxStackSize);
	return Overflow;
}

int32 UItem_Stackable::RemoveFromCount(int32 Amount)
{
	const int32 NewCount = Count - Amount;
	const int32 Underflow = FMath::Max(0, -NewCount);
	Count = FMath::Max(NewCount, 0);
	return Underflow;
}

int32 UItem_Stackable::OverrideCount(int32 NewCount)
{
	const int32 Overflow = FMath::Max(0, NewCount - MaxStackSize);
	Count = FMath::Clamp(NewCount, 0, MaxStackSize);
	return Overflow;
}

