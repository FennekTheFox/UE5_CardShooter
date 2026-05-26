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

bool UItem_Stackable::SortAmongSames(const UItem* Other) const
{
	const UItem_Stackable* OtherAsStackable = Cast<UItem_Stackable>(Other);
	if (ensure(OtherAsStackable))
	{
		return Count > OtherAsStackable->Count;
	}

	return false;
}

TArray<UItem*> UItem::SortItems(const TArray<UItem*> InItems)
{
	TArray<UItem*> OutArray = InItems;

	Algo::Sort(OutArray, [](const UItem* A, const UItem* B) {
		if (A->GetClass() != B->GetClass())
		{
			FString AClassName;
			FString BClassName;

			A->GetClass()->GetName(AClassName);
			B->GetClass()->GetName(BClassName);
			return AClassName < BClassName;
		}

		return A->SortAmongSames(B);
		});

	return OutArray;
}

