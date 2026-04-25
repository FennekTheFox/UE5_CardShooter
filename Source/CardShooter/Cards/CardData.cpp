#include "CardData.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectAbilitySystemComponent.h"
#include "Templates/SharedPointer.h"
#include "CardAttributeFragment_Base.h"

 void UCardData::GetIdentity(FCardIdentity& Identity)
{
	Identity.AssetTag = AssetTag;
	Identity.CardDataPath = FSoftObjectPath(this);
 }

 TArray<UCardEffectFragment_Attribute*> UCardData::GetAttributesOfClass(TSubclassOf<UCardEffectFragment_Attribute> Class)
 {
	 TArray<UCardEffectFragment_Attribute*> OutArray = CardAttributes;

	 OutArray = OutArray.FilterByPredicate([Class](UCardEffectFragment_Attribute* CardAttribute)
		 {
			 return CardAttribute->IsA(Class);
		 });

	 return OutArray;
 }
