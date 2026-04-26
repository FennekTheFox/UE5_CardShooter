#pragma once

#include "CardAttributeFragment_Base.generated.h"

UCLASS(BlueprintType, EditInlineNew, Abstract)
class UCardEffectFragment_Base : public UObject
{
	GENERATED_BODY()

public:
	//Returns the "label" of the fragment
	UFUNCTION(BlueprintPure)
	FText GetFragmentLabel();
};


//The base class for all root level fragments
//Supports returning a description
UCLASS(BlueprintType, Abstract)
class UCardEffectFragment_Root : public UCardEffectFragment_Base
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FText GetFragmentDescription();
};


UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_Attribute : public UCardEffectFragment_Root
{
	GENERATED_BODY()

public:

};



UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Ethereal"))
class UCardEffectFragment_Ethereal : public UCardEffectFragment_Attribute
{
	GENERATED_BODY()

public:

};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Echo"))
class UCardEffectFragment_Echo : public UCardEffectFragment_Attribute
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 EchoCount;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float EchoDelay;
};