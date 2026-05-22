// TagDrivenAnimInstance.cpp

#include "ProjectAnimationBlueprint.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UProjectAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedOwner = TryGetPawnOwner();
	if (!CachedOwner)
	{
		CachedOwner = GetOwningActor();
	}

	CacheBindings();
}

void UProjectAnimationInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedOwner)
	{
		CachedOwner = TryGetPawnOwner();
		if (!CachedOwner)
		{
			CachedOwner = GetOwningActor();
		}
	}

	for (const TPair<FGameplayTag, FBoolProperty*>& Pair : CachedBoolProperties)
	{
		const FGameplayTag& Tag = Pair.Key;
		FBoolProperty* BoolProp = Pair.Value;

		if (!BoolProp)
		{
			continue;
		}

		const bool bHasTag = OwnerHasTag(Tag);
		BoolProp->SetPropertyValue_InContainer(this, bHasTag);
	}
}

void UProjectAnimationInstance::CacheBindings()
{
	CachedBoolProperties.Empty();

	UClass* AnimClass = GetClass();

	for (const TPair<FGameplayTag, FName>& Pair : TagToBoolVariable)
	{
		const FGameplayTag& Tag = Pair.Key;
		const FName PropertyName = Pair.Value;

		if (!Tag.IsValid() || PropertyName.IsNone())
		{
			continue;
		}

		FProperty* Property = FindFProperty<FProperty>(AnimClass, PropertyName);
		FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);

		if (BoolProperty)
		{
			CachedBoolProperties.Add(Tag, BoolProperty);
		}
	}
}

bool UProjectAnimationInstance::OwnerHasTag(const FGameplayTag& Tag) const
{
	if (!CachedOwner || !Tag.IsValid())
	{
		return false;
	}

	if (const IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(CachedOwner))
	{
		return TagInterface->HasMatchingGameplayTag(Tag);
	}

	if (const IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(CachedOwner))
	{
		if (const UAbilitySystemComponent* ASC = AbilityInterface->GetAbilitySystemComponent())
		{
			return ASC->HasMatchingGameplayTag(Tag);
		}
	}

	if (const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CachedOwner))
	{
		return ASC->HasMatchingGameplayTag(Tag);
	}

	return false;
}