#pragma once

#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Item.generated.h"

USTRUCT()
struct FItemSaveData
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    TSubclassOf<UItem> ItemClass;

    UPROPERTY(SaveGame)
    TArray<uint8> Data;
};

struct FItemSaveArchive : public FObjectAndNameAsStringProxyArchive
{
    FItemSaveArchive(FArchive& InnerArchive)
        : FObjectAndNameAsStringProxyArchive(InnerArchive, true)
    {
        ArIsSaveGame = true;
        ArNoDelta = true;
    }
};

UCLASS(BlueprintType)
class UItem : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, BlueprintNativeEvent)
    void GetItemBrush(FSlateBrush& OutBrush);

    UFUNCTION(BlueprintPure, BlueprintNativeEvent)
    bool SameItem(const UItem* Other) const;

    UFUNCTION(BlueprintPure, BlueprintNativeEvent)
    FText GetItemName();
    UFUNCTION(BlueprintPure, BlueprintNativeEvent)
    FText GetItemDescription();

    virtual bool SortAmongSames(const UItem* Other) const { return ensure(false); };

    UFUNCTION(BlueprintCallable)
    static TArray<UItem*> SortItems(const TArray<UItem*> InItems);
};

UCLASS(BlueprintType)
class UItem_Stackable : public UItem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 Count = 1;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    int32 MaxStackSize = 9999;

    //returns the amount exceeding the stack, so 0 = all fit, >0 = leftovers
    UFUNCTION(BlueprintCallable)
    int32 AddToCount(int32 Amount);

    UFUNCTION(BlueprintCallable)
    int32 RemoveFromCount(int32 Amount);

    //If given an amount > MaxCount, returns the overflow
    UFUNCTION(BlueprintCallable)
    int32 OverrideCount(int32 NewCount);

    UFUNCTION(BlueprintPure)
    bool IsFullStack() { return Count == MaxStackSize; };


    virtual bool SortAmongSames(const UItem* Other) const override;
};