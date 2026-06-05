#pragma once

#include "GameplayTagContainer.h"

#include "CardAugmentData.generated.h"

USTRUCT(BlueprintType)
struct FCardAugmentParameter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Augments")
	FGameplayTag ParameterTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Augments")
	float Magnitude = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Augments")
	int32 VariantOption = 0;

	bool operator== (const FCardAugmentParameter& Other) const
	{
		return FMath::IsNearlyEqual(Magnitude, Other.Magnitude)
			&& VariantOption == Other.VariantOption
			&& ParameterTag == Other.ParameterTag;
	}

	bool operator<(const FCardAugmentParameter& Other) const
	{
		if (ParameterTag != Other.ParameterTag)
		{
			return ParameterTag.ToString() < Other.ParameterTag.ToString();
		}

		if (!FMath::IsNearlyEqual(Magnitude, Other.Magnitude))
		{
			return Magnitude < Other.Magnitude;
		}

		return VariantOption < Other.VariantOption;
	}
};

FORCEINLINE uint32 GetTypeHash(const FCardAugmentParameter& Param)
{
	uint32 Hash = GetTypeHash(Param.VariantOption);
	Hash = HashCombine(Hash, GetTypeHash(Param.Magnitude));
	Hash = HashCombine(Hash, GetTypeHash(Param.ParameterTag));
	return Hash;
}

USTRUCT(BlueprintType)
struct FCardAugmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Augments")
	FGameplayTag AugmentTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Augments")
	FSoftClassPath AugmentClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Augments")
	TArray<FCardAugmentParameter> AugmentParameters;

	bool operator== (const FCardAugmentData& Other) const
	{
		if (AugmentTag != Other.AugmentTag)
		{
			return false;
		}

		if (AugmentClass != Other.AugmentClass)
		{
			return false;
		}

		if (AugmentParameters.Num() != Other.AugmentParameters.Num())
		{
			return false;
		}

		for (int i = 0; i < AugmentParameters.Num(); i++)
		{
			const FCardAugmentParameter& AugmentParam = AugmentParameters[i];
			const FCardAugmentParameter& OtherAugmentParam = Other.AugmentParameters[i];

			if (AugmentParam != OtherAugmentParam)
			{
				return false;
			}
		}

		return true;
	}

	bool operator<(const FCardAugmentData& Other) const
	{
		//AugmentTag
		if (AugmentTag != Other.AugmentTag)
		{
			return AugmentTag.ToString() < Other.AugmentTag.ToString();
		}

		// AugmentClass
		if (AugmentClass != Other.AugmentClass)
		{
			return AugmentClass.ToString() < Other.AugmentClass.ToString();
		}

		// Num params
		if (AugmentParameters.Num() != Other.AugmentParameters.Num())
		{
			return AugmentParameters.Num() < Other.AugmentParameters.Num();
		}

		for (int32 i = 0; i < AugmentParameters.Num(); ++i)
		{
			// Compare values
			const FCardAugmentParameter& ValueA = AugmentParameters[i];
			const FCardAugmentParameter& ValueB = Other.AugmentParameters[i];

			if (!(ValueA == ValueB))
			{
				return ValueA < ValueB;
			}
		}

		return false;
	}
};

FORCEINLINE uint32 GetTypeHash(const FCardAugmentData& Data)
{
	uint32 Hash = GetTypeHash(Data.AugmentClass);
	//Hash = HashCombine(Hash, GetTypeHash(Data.AugmentClass));

	for (const FCardAugmentParameter& Parameter : Data.AugmentParameters)
	{
		Hash = HashCombine(Hash, GetTypeHash(Parameter));
	}

	return Hash;
}