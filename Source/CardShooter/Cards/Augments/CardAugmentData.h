#pragma once

#include "GameplayTagContainer.h"

#include "CardAugmentData.generated.h"


USTRUCT(BlueprintType)
struct FCardAugmentParameter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Augments")
	float Magnitude = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, Category = "Augments")
	int32 VariantOption = 0;

	bool operator== (const FCardAugmentParameter& Other) const
	{
		return FMath::IsNearlyEqual(Magnitude, Other.Magnitude)
			&& VariantOption == Other.VariantOption;
	}

	bool operator<(const FCardAugmentParameter& Other) const
	{
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
	TMap<FGameplayTag, FCardAugmentParameter> AugmentParameters;

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

		for (const TPair<FGameplayTag, FCardAugmentParameter>& Pair : AugmentParameters)
		{
			const FCardAugmentParameter* OtherValue =
				Other.AugmentParameters.Find(Pair.Key);

			if (!OtherValue || !(Pair.Value == *OtherValue))
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

		// Deterministic compare for TMap
		TArray<FGameplayTag> KeysA;
		TArray<FGameplayTag> KeysB;

		AugmentParameters.GetKeys(KeysA);
		Other.AugmentParameters.GetKeys(KeysB);

		KeysA.Sort([](const FGameplayTag& A, const FGameplayTag& B)
			{
				return A.ToString() < B.ToString();
			});

		KeysB.Sort([](const FGameplayTag& A, const FGameplayTag& B)
			{
				return A.ToString() < B.ToString();
			});

		for (int32 i = 0; i < KeysA.Num(); ++i)
		{
			// Compare keys
			if (KeysA[i] != KeysB[i])
			{
				return KeysA[i].ToString() < KeysB[i].ToString();
			}

			// Compare values
			const FCardAugmentParameter& ValueA = AugmentParameters[KeysA[i]];
			const FCardAugmentParameter& ValueB = Other.AugmentParameters[KeysB[i]];

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

	for (const auto& Pair : Data.AugmentParameters)
	{
		Hash = HashCombine(Hash, GetTypeHash(Pair.Key));
		Hash = HashCombine(Hash, GetTypeHash(Pair.Value));
	}

	return Hash;
}