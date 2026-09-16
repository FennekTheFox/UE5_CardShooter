#pragma once

#include "Cards/CardIdentity.h"
#include "CardExecution_AdditionalData.h"
#include "UI/RichTextHoverDecorator.h"
#include "Misc/NeonDamageType.h"
#include "CardAttributeFragment_Base.generated.h"

UCLASS(BlueprintType, EditInlineNew, Abstract)
class UCardEffectFragment_Base : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	bool GetShouldShowInDescription();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	FText GetLocalizableName(const FCardIdentity& CardIdentity);	

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	FText GetLocalizableDescription(const FCardIdentity& CardIdentity) const;
	virtual FText GetRawLocalizableDescription(const FCardIdentity& CardIdentity) const;

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

UCLASS(Blueprintable, Abstract, EditInlineNew)
class UCardParameter : public UObject,
	public IHoverableContext
{
	GENERATED_BODY()

public: 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ParameterTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FSetByCallerMetaData MetaData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bNeedsLocalization = true;

	//BEGIN IHoverableContext
	void GetHoverText_Implementation(FText& OutText) const override;
	//END IHoverableContext

	virtual FText GetStyle(const FSetByCallerMetaData& InMetaData) { return FText::GetEmpty(); }
};

UCLASS(Blueprintable, Abstract, EditInlineNew)
class UCardParameter_ScalingGroupParameter : public UCardParameter
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FText> ScalingGroupNames;

	//BEGIN IHoverableContext
	void GetHoverContentTitle_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const UCardEffectFragment_Execution_WithParameters* RelatedExec = nullptr) const override;
	void GetHoverContent_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const FCardIdentity& CardIdentity, const UCardEffectFragment_Execution_WithParameters* RelatedExec = nullptr) const override;
	//END IHoverableContext

	FText GetStyle(const FSetByCallerMetaData& InMetaData) override;

};

UCLASS(Blueprintable, Abstract, EditInlineNew)
class UCardParameter_DamageType : public UCardParameter
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int32, TSubclassOf<UNeonDamageType>> DamageTypeMap;

	//BEGIN IHoverableContext
	void GetHoverContentTitle_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const UCardEffectFragment_Execution_WithParameters* RelatedExec = nullptr) const override;
	void GetHoverContent_Implementation(const FSetByCallerMetaData& InMetaData, FText& OutText, const FCardIdentity& CardIdentity, const UCardEffectFragment_Execution_WithParameters* RelatedExec = nullptr) const override;
	//END IHoverableContext

	FText GetStyle(const FSetByCallerMetaData& InMetaData) override;
};

UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_Attribute : public UCardEffectFragment_Root
{
	GENERATED_BODY()

public:
	UCardEffectFragment_Attribute();

	UPROPERTY(BlueprintReadOnly)
	FText AttributeName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Instanced)
	TArray<UCardParameter*> Parameters;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag AttributeTag;

	virtual void GetFormattingArgs(FFormatNamedArguments& Args, const FCardIdentity& CardIdentity) const;

	UFUNCTION(BlueprintPure)
	bool GetParameterValue(FGameplayTag ParamTag, float& OutValue, const FCardIdentity& CardIdentity) const;

	UFUNCTION(BlueprintPure)
	bool GetCanBeExtracted() const {return bCanBeExtracted;}

protected:
	bool bCanBeExtracted = false;
};

//Parent class for attributes that can be applied to an execution
UCLASS(BlueprintType, Blueprintable, Abstract)
class UCardEffectFragment_ExecutionAttribute : public UCardEffectFragment_Attribute
{
	GENERATED_BODY()

public:

};
