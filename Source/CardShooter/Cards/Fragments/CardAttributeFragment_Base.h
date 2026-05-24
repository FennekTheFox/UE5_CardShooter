#pragma once

#include "Cards/CardIdentity.h"
#include "CardAttributeFragment_Base.generated.h"

UCLASS(BlueprintType, EditInlineNew, Abstract)
class UCardEffectFragment_Base : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	bool GetShouldShowInDescription();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	FText GetLocalizableName();	

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	FText GetLocalizableDescription(const FCardIdentity& CardIdentity);
	virtual FText GetRawLocalizableDescription(const FCardIdentity& CardIdentity);

protected:
	bool bShowInDescription = true;
};


//The base class for all root level fragments
//Supports returning a description
UCLASS(BlueprintType, Abstract)
class UCardEffectFragment_Root : public UCardEffectFragment_Base
{
	GENERATED_BODY()

public:
};


UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_Attribute : public UCardEffectFragment_Root
{
	GENERATED_BODY()

public:

};

//Parent class for attributes that can be applied to an execution
UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_ExecutionAttribute : public UCardEffectFragment_Attribute
{
	GENERATED_BODY()

public:

};
