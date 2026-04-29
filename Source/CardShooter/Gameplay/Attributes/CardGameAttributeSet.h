#pragma once

#include "ProjectAttributeSet.h"

#include "CardGameAttributeSet.generated.h"

UCLASS()
class UCardGameAttributeSet : public UProjectAttributeSet
{
	GENERATED_BODY()

public:
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	ATTRIBUTE_ACCESSORS(UCardGameAttributeSet, DrawCount);
	ATTRIBUTE_ACCESSORS(UCardGameAttributeSet, DrawDelay);
	ATTRIBUTE_ACCESSORS(UCardGameAttributeSet, ShuffleDelay);
	ATTRIBUTE_ACCESSORS(UCardGameAttributeSet, CardPlayCooldown);

	UFUNCTION()
	void OnRep_DrawCount(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_DrawDelay(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_ShuffleDuration(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_CardPlayCooldown(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DrawCount, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DrawCount;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DrawDelay, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DrawDelay;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ShuffleDuration, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShuffleDelay;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CardPlayCooldown, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CardPlayCooldown;
};