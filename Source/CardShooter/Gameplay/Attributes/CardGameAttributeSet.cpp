
#include "CardGameAttributeSet.h"

#include "Net/UnrealNetwork.h"

void UCardGameAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCardGameAttributeSet, DrawCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCardGameAttributeSet, DrawDelay, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCardGameAttributeSet, ShuffleDelay, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCardGameAttributeSet, CardPlayCooldown, COND_None, REPNOTIFY_Always);
}

void UCardGameAttributeSet::OnRep_DrawCount(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCardGameAttributeSet, DrawCount, OldValue);
}

void UCardGameAttributeSet::OnRep_DrawDelay(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCardGameAttributeSet, DrawDelay, OldValue);
}

void UCardGameAttributeSet::OnRep_ShuffleDuration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCardGameAttributeSet, ShuffleDelay, OldValue);
}

void UCardGameAttributeSet::OnRep_CardPlayCooldown(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCardGameAttributeSet, CardPlayCooldown, OldValue);
}

