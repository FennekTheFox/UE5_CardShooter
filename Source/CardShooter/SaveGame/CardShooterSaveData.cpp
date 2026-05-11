#include "CardShooterSaveData.h"
#include "Util/CardShooterUtility.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "Currency.h"
#include "Kismet/GameplayStatics.h"
#include "CardShooterSaveGameSubsystem.h"
#include "Engine/LocalPlayer.h"

static FAutoConsoleCommandWithWorldAndArgs GiveCurrencyCmd(
	TEXT("GiveCurrency"),
	TEXT("Give currency to the player: GiveCurrency <Currency> <Amount> <Player Index>"),
	FConsoleCommandWithWorldAndArgsDelegate::CreateLambda(
		[](const TArray<FString>& Args, UWorld* World)
		{
			if (Args.Num() < 3) return;

			int32 Amount = FCString::Atoi(*Args[1]);
			int32 PlayerIndex = FCString::Atoi(*Args[2]);
			FGameplayTag CurrencyTag;

			FAssetRegistryModule& AssetRegistryModule =
				FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

			FARFilter Filter;
			Filter.ClassPaths.Add(UCurrencyDef::StaticClass()->GetClassPathName());
			Filter.bRecursiveClasses = true;

			TArray<FAssetData> Assets;
			AssetRegistryModule.Get().GetAssets(Filter, Assets);

			for (const FAssetData& Asset : Assets)
			{
				FString Value;
				if (Asset.GetTagValue(TEXT("CommandString"), Value))
				{
					if (Value == Args[0])
					{
						UCurrencyDef* CurrencyDef = Cast<UCurrencyDef>(Asset.GetAsset());
						CurrencyTag = CurrencyDef->CurrencyTag;
					}
				}
			}

			if (!CurrencyTag.IsValid()) return;

			if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, PlayerIndex))
			{
				if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
				{
					UCardShooterSaveGameSubsystem* SaveGameSubsystem = LocalPlayer->GetSubsystem<UCardShooterSaveGameSubsystem>();

					if (UCardShooterSaveData* SaveData = SaveGameSubsystem->GetLocalPlayerSaveData())
					{
						SaveData->AddCurrency(CurrencyTag, Amount);
					}
				}
			}
		}
	)
);


void UCardShooterSaveData::ValidateAndFix(FCardIdentity& CardIdentity)
{
	//Also correct the asset tag in the card identity just to be sure

	UCardData* Data = UCardShooterUtilityLibrary::GetCardDataFromIdentity(CardIdentity);
	if (ensure(Data))
	{
		CardIdentity.AssetTag = Data->AssetTag;
	}
}

void UCardShooterSaveData::AddCardToCollection(const FCardIdentity& CardIdentity)
{
	FCardIdentity FixedIdentity = CardIdentity;
	ValidateAndFix(FixedIdentity);

	int32& Count = Collection.Cards.FindOrAdd(FixedIdentity, 0);
	Count += 1;
}

bool UCardShooterSaveData::RemoveCardFromCollection(const FCardIdentity& CardIdentity)
{
	FCardIdentity FixedIdentity = CardIdentity;
	ValidateAndFix(FixedIdentity);

	if (int32* Count = Collection.Cards.Find(FixedIdentity))
	{
		if (*Count == 1)
		{
			Collection.Cards.Remove(FixedIdentity);
		}
		else
		{
			Collection.Cards.Add(FixedIdentity, *Count - 1);
		}

		return true;
	}
	else
	{
		//Card not contained
		return false;
	}
}

void UCardShooterSaveData::GetSortedCardCollectionKeys(TArray<FCardIdentity>& OutCards)
{
	Collection.Cards.GetKeys(OutCards);

	OutCards.Sort([](const FCardIdentity& A, const FCardIdentity& B) {
		if (A.AssetTag != B.AssetTag)
		{
			return A.AssetTag.ToString() < B.AssetTag.ToString();
		}
		if (A.CardRarity != B.CardRarity)
		{
			return A.CardRarity < B.CardRarity;
		}
		if (A.bFullArt != B.bFullArt)
		{
			return B.bFullArt;
		}

		ensure(false);
		return false;
		});
}

void UCardShooterSaveData::AddCurrency(FGameplayTag CurrencyTag, int32 Amount)
{
	int32& HaveAmount = Currencies.FindOrAdd(CurrencyTag, 0);
	HaveAmount += Amount;
}

bool UCardShooterSaveData::DeductCurrency(FGameplayTag CurrencyTag, int32 Amount)
{
	int32& HaveAmount = Currencies.FindOrAdd(CurrencyTag, 0);

	if (HaveAmount >= Amount)
	{
		HaveAmount -= Amount;
		return true;
	}

	return false;
}

int32 UCardShooterSaveData::GetCurrencyAmount(FGameplayTag CurrencyTag)
{
	int32& HaveAmount = Currencies.FindOrAdd(CurrencyTag, 0);
	return HaveAmount;
}

