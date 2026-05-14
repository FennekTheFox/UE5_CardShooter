#include "CardShooterUtility.h"
#include "Cards/Fragments/CardAttributeFragment_DeckLimit.h"
#include "Currency.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "UObject/UnrealType.h"
#include "UObject/Stack.h"

UCardData* UCardShooterUtilityLibrary::GetCardDataFromIdentity(const FCardIdentity& Identity)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FAssetData AssetData;
	AssetRegistry.TryGetAssetByObjectPath(Identity.CardDataPath, AssetData);
	if (AssetData.IsValid())
	{
		//We found a valid card data
		if (UCardData* LoadedCard = Cast<UCardData>(AssetData.GetAsset()))
		{
			if (!Identity.AssetTag.IsValid() ||LoadedCard->AssetTag == Identity.AssetTag)
			{
				return LoadedCard;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Warning: Unable to find card data with matching tag at path\"%s\""), *Identity.CardDataPath.ToString())

	//We couldnt retrieve a valid card by soft path, so lets check if we can find it at all
	TArray<FAssetData> CardAssets;
	AssetRegistry.GetAssetsByClass(UCardData::StaticClass()->GetClassPathName(), CardAssets, true);
	{
		for (FAssetData& AssetDataIt : CardAssets)
		{
			//We found a valid card data
			if (UCardData* LoadedCard = Cast<UCardData>(AssetDataIt.GetAsset()))
			{
				if (LoadedCard->AssetTag == Identity.AssetTag)
				{
					return LoadedCard;
				}
			}
		}
	}

	return nullptr;
}

bool UCardShooterUtilityLibrary::CanAddCardToDeck(const FCardDeck& Deck, UCardData* CardToAdd, TArray<UDeckLimitationViolation*>& FailureReasons)
{
	FailureReasons.Empty();

	if (!CardToAdd)
	{
		return false;
	}

	TArray<UCardData*> PostChangeDeck;
	PostChangeDeck.Add(CardToAdd);

	for (const FCardIdentity& Identity : Deck.CardList)
	{
		PostChangeDeck.Add(GetCardDataFromIdentity(Identity));
	}

	return VerifyDeck(PostChangeDeck, FailureReasons);
}

bool UCardShooterUtilityLibrary::VerifyDeck(const TArray<UCardData*> Deck, TArray<class UDeckLimitationViolation*>& FailureReasons)
{
	bool bSuccess = true;

	for (UCardData* Card : Deck)
	{
		if (!Card)
		{
			return false;
		}

		for (UCardEffectFragment_Attribute* DeckLimitation : Card->CardAttributes)
		{
			if (UCardEffectFragment_DeckLimitation* AsDeckLimitation = Cast<UCardEffectFragment_DeckLimitation>(DeckLimitation))
			{
				bSuccess &= AsDeckLimitation->Verify(Deck, FailureReasons);
			}
		}
	}

	return bSuccess;
}

bool UCardShooterUtilityLibrary::StringLexComparison(const FString& A, const FString& B)
{
	return A < B; // lexicographic
}

void UCardShooterUtilityLibrary::BindEventToTagCountChanged(class UAbilitySystemComponent* ASC, FGameplayTag Tag, FOnGameplayTagChangedBP Callback)
{
	if (!ensure(ASC))
	{
		return;
	}

	ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::AnyCountChange).AddLambda([Tag, Callback](FGameplayTag ChangedTag, int32 NewCount) {
		if (ChangedTag == Tag)
		{
			Callback.ExecuteIfBound(ChangedTag, NewCount);
		}
		});
}

class UCardEffectFragment_Attribute* UCardShooterUtilityLibrary::GetAttributeOfClassFromArray(TArray<UCardEffectFragment_Attribute*> Array, TSubclassOf<UCardEffectFragment_Attribute> Class)
{
	for (UCardEffectFragment_Attribute* Attribute : Array)
	{
		if (!ensure(Attribute))
		{
			continue;
		}

		if (Attribute->IsA(Class))
		{
			return Attribute;
		}
	}
	return nullptr;
}

bool UCardShooterUtilityLibrary::Equal_CardIdentity(const FCardIdentity& A, const FCardIdentity& B)
{
	return A == B;
}

AActor* UCardShooterUtilityLibrary::GetInstigatorFromEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (!Handle.IsValid())
	{
		return nullptr;
	}

	FGameplayEffectContextHandle Context = Handle.GetOwningAbilitySystemComponent()->GetEffectContextFromActiveGEHandle(Handle);

	return Context.GetOriginalInstigator();
}

void UCardShooterUtilityLibrary::ResetDurationOnEffect(const FActiveGameplayEffectHandle& Handle)
{
	UAbilitySystemComponent* ASC = Handle.GetOwningAbilitySystemComponent();

	if (!ASC || !Handle.IsValid())
	{
		return;
	}

	const FActiveGameplayEffect* ActiveEffect =
		ASC->GetActiveGameplayEffect(Handle);

	if (!ActiveEffect)
	{
		return;
	}

	// Copy spec before removing
	FGameplayEffectSpec SpecCopy = ActiveEffect->Spec;

	ASC->RemoveActiveGameplayEffect(Handle);

	ASC->ApplyGameplayEffectSpecToSelf(SpecCopy);
}

void UCardShooterUtilityLibrary::AddSourceToContext(FGameplayEffectContextHandle Handle, UObject* Source)
{
	Handle.AddSourceObject(Source);
}

class UCurrencyDef* UCardShooterUtilityLibrary::FindCurrencyDef(FGameplayTag CurrencyTag)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	//We couldnt retrieve a valid card by soft path, so lets check if we can find it at all
	TArray<FAssetData> CurrencyAssets;
	AssetRegistry.GetAssetsByClass(UCurrencyDef::StaticClass()->GetClassPathName(), CurrencyAssets, true);
	{
		for (FAssetData& AssetDataIt : CurrencyAssets)
		{
			//We found a valid card data
			if (UCurrencyDef* LoadedCurrency = Cast<UCurrencyDef>(AssetDataIt.GetAsset()))
			{
				if (LoadedCurrency->CurrencyTag == CurrencyTag)
				{
					return LoadedCurrency;
				}
			}
		}
	}

	return nullptr;
}

void UCardShooterUtilityLibrary::SortCardIdentityArray(TArray<FCardIdentity>& Array, FCardIdentitySortPredicate Predicate)
{
	Array.Sort(
		[&Predicate](const FCardIdentity& A, const FCardIdentity& B)
		{
			if (Predicate.IsBound())
			{
				return Predicate.Execute(A, B);
			}

			return false;
		});
}
