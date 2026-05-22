// TagDrivenAnimInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "ProjectAnimationBlueprint.generated.h"

UCLASS(Blueprintable)
class UProjectAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	// Key = required tag on owning actor
	// Value = bool variable name on this AnimInstance / AnimBP
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	TMap<FGameplayTag, FName> TagToBoolVariable;

private:
	UPROPERTY(Transient)
	TObjectPtr<AActor> CachedOwner;

	TMap<FGameplayTag, FBoolProperty*> CachedBoolProperties;

	void CacheBindings();
	bool OwnerHasTag(const FGameplayTag& Tag) const;
};