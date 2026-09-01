#include "CardShooterUtility.h"
#include "Cards/Fragments/CardAttributeFragment_DeckLimit.h"
#include "Currency.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagsManager.h"
#include "SaveGame/CardShooterSaveGameSubsystem.h"
#include "SaveGame/CardShooterSaveData.h"
#include "Internationalization/Regex.h"
#include "Cards/CardData.h"

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
			if (!Identity.AssetTag.IsValid() || LoadedCard->AssetTag == Identity.AssetTag)
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

//bool UCardShooterUtilityLibrary::CanAddCardToDeck(const FCardDeck& Deck, FCardIdentity* CardToAdd, TArray<class UDeckLimitationViolation*>& FailureReasons)
//{
//	FailureReasons.Empty();
//
//	if (!CardToAdd)
//	{
//		return false;
//	}
//
//	FCardDeck PostChangeDeck = Deck;
//	PostChangeDeck.CardList.Add(CardToAdd);
//
//	return VerifyDeck(PostChangeDeck, FailureReasons);
//}

bool UCardShooterUtilityLibrary::VerifyDeck(const FCardDeck& Deck, APlayerController* PlayerController, TArray<class UDeckLimitationViolation*>& FailureReasons)
{
	if (!ensure(PlayerController))
	{
		return false;
	}

	bool bSuccess = true;

	FCardDeck_DataOnly DataOnlyDeck;

	for (const FCardIdentityHandle& CardHandle : Deck.UtilityCards)
	{
		FCardIdentity Identity;
		//no ensure here bc utility cards may have invalid handles
		if (CardHandle == FCardIdentityHandle::Invalid)
		{
			continue;
		}

		if (!(UCardIdentityHandleLibrary::GetCardIdentityFromHandle(CardHandle, Identity, PlayerController)))
		{
			continue;
		}
		DataOnlyDeck.UtilityCards.Add(GetCardDataFromIdentity(Identity));
	}

	for (const FCardIdentityHandle& CardHandle : Deck.CardHandles)
	{
		FCardIdentity Identity;
		if (!(UCardIdentityHandleLibrary::GetCardIdentityFromHandle(CardHandle, Identity, PlayerController)))
		{
			return false;
		}
		DataOnlyDeck.CardList.Add(GetCardDataFromIdentity(Identity));
	}

	for (UCardData* Card : DataOnlyDeck.UtilityCards)
	{
		if (!Card)
		{
			return false;
		}

		for (UCardEffectFragment_Attribute* DeckLimitation : Card->CardAttributes)
		{
			if (UCardEffectFragment_DeckLimitation* AsDeckLimitation = Cast<UCardEffectFragment_DeckLimitation>(DeckLimitation))
			{
				bSuccess &= AsDeckLimitation->Verify(DataOnlyDeck, FailureReasons);
			}
		}
	}

	for (UCardData* Card : DataOnlyDeck.CardList)
	{
		if (!Card)
		{
			return false;
		}

		for (UCardEffectFragment_Attribute* DeckLimitation : Card->CardAttributes)
		{
			if (UCardEffectFragment_DeckLimitation* AsDeckLimitation = Cast<UCardEffectFragment_DeckLimitation>(DeckLimitation))
			{
				bSuccess &= AsDeckLimitation->Verify(DataOnlyDeck, FailureReasons);
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

bool UCardShooterUtilityLibrary::Equal_CardIdentityHandle(const FCardIdentityHandle& A, const FCardIdentityHandle& B)
{
	return A == B;
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

UCardAugment* UCardShooterUtilityLibrary::GetCardAugmentCDO(const FCardAugmentData& AugmentData)
{// 1. Try soft class path first
	if (AugmentData.AugmentClass.IsValid())
	{
		if (UClass* LoadedClass = AugmentData.AugmentClass.TryLoadClass<UCardAugment>())
		{
			if (LoadedClass->IsChildOf(UCardAugment::StaticClass()))
			{
				return LoadedClass->GetDefaultObject<UCardAugment>();
			}
		}
	}

	// 2. Fallback: find loaded subclasses by matching AugmentTag on their CDO
	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Class = *It;

		if (!Class || Class->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists))
		{
			continue;
		}

		if (!Class->IsChildOf(UCardAugment::StaticClass()))
		{
			continue;
		}

		UCardAugment* CDO = Class->GetDefaultObject<UCardAugment>();
		if (!CDO)
		{
			continue;
		}

		if (CDO->AugmentTag == AugmentData.AugmentTag)
		{
			return CDO;
		}
	}

	return nullptr;
}

void UCardShooterUtilityLibrary::GetChildGameplayTags(FGameplayTag ParentTag, TArray<FGameplayTag>& ChildTags, bool bIncludeParent /*= false*/)
{
	if (!ParentTag.IsValid())
	{
		return;
	}

	FGameplayTagContainer TagContainer = UGameplayTagsManager::Get().RequestGameplayTagChildren(ParentTag);

	for (const FGameplayTag& Tag : TagContainer)
	{
		if (!bIncludeParent && Tag == ParentTag)
		{
			continue;
		}

		ChildTags.Add(Tag);
	}

	ChildTags.Sort([](const FGameplayTag& A, const FGameplayTag& B)
		{
			return A.ToString() < B.ToString();
		});
}

TArray<FGameplayTag> UCardShooterUtilityLibrary::GetCardSetByCallerTags(const FCardIdentity& Identity)
{
	TArray<FGameplayTag> Result;

	const UCardData* CardData = GetCardDataFromIdentity(Identity);
	if (!CardData)
	{
		return Result;
	}

	TSet<FGameplayTag> UniqueTags;

	TFunction<void(const UCardEffectFragment_Execution*)> GatherTags =
		[&UniqueTags, &GatherTags](const UCardEffectFragment_Execution* Execution)
		{
			if (!Execution)
			{
				return;
			}

			if (const UCardEffectFragment_Execution_WithParameters* ParamExecution =
				Cast<UCardEffectFragment_Execution_WithParameters>(Execution))
			{
				for (const UCardParameter* Param : ParamExecution->Parameters)
				{
					if (!Param)
					{
						continue;
					}
					UniqueTags.Add(Param->ParameterTag);
				}
			}

			if (const UCardEffectFragment_Execution_Compound* CompoundExecution =
				Cast<UCardEffectFragment_Execution_Compound>(Execution))
			{
				for (const UCardEffectFragment_Execution* ChildExecution : CompoundExecution->Executions)
				{
					GatherTags(ChildExecution);
				}
			}
		};

	GatherTags(CardData->PrimaryExecution);
	GatherTags(CardData->SecondaryExecution);

	Result = UniqueTags.Array();

	Result.Sort([](const FGameplayTag& A, const FGameplayTag& B)
		{
			return A.ToString() < B.ToString();
		});

	return Result;
}

FText UCardShooterUtilityLibrary::FormatTextForDisplay(const FText& Text)
{
	//DamageTypes
	FString Result = Text.ToString();

	//const TSet<FString> ValidDamageTypes =
	//{
	//	TEXT("Poison"),
	//	TEXT("Fire"),
	//	TEXT("Cold"),
	//	TEXT("Lightning"),
	//	TEXT("Physical")
	//};

	//for (const FString& DamageType : ValidDamageTypes)
	//{
	//	const FString Token = FString::Printf(
	//		TEXT("<%s>"),
	//		*DamageType
	//	);

	//	const FString Replacement = FString::Printf(
	//		TEXT("<img id=\"%s\"/> <DamageType%s>%s</>"),
	//		*DamageType,
	//		*DamageType,
	//		*DamageType
	//	);

	//	Result.ReplaceInline(
	//		*Token,
	//		*Replacement,
	//		ESearchCase::CaseSensitive
	//	);
	//}

	//const FRegexPattern Pattern(TEXT(R"(\{[^}]*Magnitude\})"));
	//FRegexMatcher Matcher(Pattern, Result);

	//TArray<FString> Matches;

	//while (Matcher.FindNext())
	//{
	//	Matches.Add(Matcher.GetCaptureGroup(0));
	//}

	//for (const FString& Match : Matches)
	//{
	//	const FString Replacement = FString::Printf(
	//		TEXT("<SetByCallerValue>%s</>"),
	//		*Match
	//	);

	//	Result.ReplaceInline(
	//		*Match,
	//		*Replacement,
	//		ESearchCase::CaseSensitive
	//	);
	//}


	return FText::FromString(Result);
}

TArray<FGameplayTag> UCardShooterUtilityLibrary::GetCardScalingGroups(const FCardIdentity& Identity)
{
	TArray<FGameplayTag> Result;

	const UCardData* CardData = GetCardDataFromIdentity(Identity);
	if (!CardData)
	{
		return Result;
	}

	CardData->GetAllUsedScalingGroups(Result);

	Result.Sort([](const FGameplayTag& A, const FGameplayTag& B)
		{
			return A.ToString() < B.ToString();
		});

	return Result;
}

void UCardShooterUtilityLibrary::FilterObjectsByClass(const TArray<UObject*>& Objects, TSubclassOf<UObject> Class, TArray<UObject*>& OutObjects)
{
	OutObjects.Reset();

	if (!Class)
	{
		return;
	}

	for (UObject* Object : Objects)
	{
		if (IsValid(Object) && Object->IsA(Class))
		{
			OutObjects.Add(Object);
		}
	}
}

bool UCardShooterUtilityLibrary::GetSpecFromActiveEffect(const FActiveGameplayEffectHandle ActiveEffect, class UAbilitySystemComponent* OwningASC, FGameplayEffectSpec& OutSpec)
{
	if (!OwningASC || !ActiveEffect.IsValid())
	{
		return false;
	}

	const FActiveGameplayEffect* AGE =
		OwningASC->GetActiveGameplayEffect(ActiveEffect);

	if (!AGE)
	{
		return false;
	}

	OutSpec = AGE->Spec;
	return true;
}

void UCardShooterUtilityLibrary::DuplicateSpec(class UAbilitySystemComponent* ASC, const FGameplayEffectSpec& InSpec, FGameplayEffectSpecHandle& OutSpec)
{
	if (!ASC || !InSpec.Def)
	{
		OutSpec = FGameplayEffectSpecHandle();
		return;
	}

	OutSpec = ASC->MakeOutgoingSpec(
		InSpec.Def->GetClass(),
		InSpec.GetLevel(),
		InSpec.GetContext()
	);

	if (!OutSpec.IsValid())
	{
		return;
	}

	FGameplayEffectSpec* NewSpec = OutSpec.Data.Get();

	NewSpec->CopySetByCallerMagnitudes(InSpec);
	NewSpec->SetDuration(InSpec.GetDuration(), true);

	// Optional depending on your needs:
	NewSpec->SetStackCount(InSpec.GetStackCount());
}

UObject* UCardShooterUtilityLibrary::DuplicateObjectTyped(UObject* SourceObject, UObject* NewOuter)
{
	if (!SourceObject)
		return nullptr;

	if (!NewOuter)
		NewOuter = GetTransientPackage();

	return DuplicateObject(SourceObject, NewOuter);
}


DEFINE_FUNCTION(UCardShooterUtilityLibrary::execWeightedRandomMapIndex)
{
	Stack.StepCompiledIn<FMapProperty>(nullptr);

	const FMapProperty* MapProperty = CastField<FMapProperty>(Stack.MostRecentProperty);

	void* MapAddr = Stack.MostRecentPropertyAddress;

	P_FINISH;

	int32 Result = INDEX_NONE;

	if (!MapProperty || !MapAddr)
	{
		*(int32*)RESULT_PARAM = Result;
		return;
	}

	const FNumericProperty* NumericProp =
		CastField<FNumericProperty>(MapProperty->ValueProp);

	if (!NumericProp || !NumericProp->IsFloatingPoint())
	{
		*(int32*)RESULT_PARAM = Result;
		return;
	}

	FScriptMapHelper MapHelper(MapProperty, MapAddr);

	float TotalWeight = 0.f;

	for (int32 Index = 0; Index < MapHelper.GetMaxIndex(); ++Index)
	{
		if (!MapHelper.IsValidIndex(Index))
		{
			continue;
		}

		const float Weight = static_cast<float>(
			NumericProp->GetFloatingPointPropertyValue(
				MapHelper.GetValuePtr(Index)));

		TotalWeight += FMath::Max(0.f, Weight);
	}

	if (TotalWeight <= 0.f)
	{
		*(int32*)RESULT_PARAM = Result;
		return;
	}

	float Roll = FMath::FRandRange(0.f, TotalWeight);

	int32 EntryIndex = 0;

	for (int32 Index = 0; Index < MapHelper.GetMaxIndex(); ++Index)
	{
		if (!MapHelper.IsValidIndex(Index))
		{
			continue;
		}

		const float Weight = static_cast<float>(
			NumericProp->GetFloatingPointPropertyValue(
				MapHelper.GetValuePtr(Index)));

		Roll -= FMath::Max(0.f, Weight);

		if (Roll <= 0.f)
		{
			Result = EntryIndex;
			break;
		}

		++EntryIndex;
	}

	*(int32*)RESULT_PARAM = Result;
}

bool UCardShooterUtilityLibrary::GetActiveEffectThatGrantedAbility(UGameplayAbility* AbilityInstance, FActiveGameplayEffectHandle& OutActiveEffectHandle)
{
	if (!AbilityInstance)
	{
		return false;
	}

	FGameplayAbilitySpec* AbilitySpec = AbilityInstance->GetCurrentAbilitySpec();

	if (!AbilitySpec)
	{
		return false;
	}

	FActiveGameplayEffectHandle OriginEffectHandle = AbilitySpec->GameplayEffectHandle;

	if (!OriginEffectHandle.IsValid())
	{
		return false;
	}

	OutActiveEffectHandle = OriginEffectHandle;
	return true;
}
