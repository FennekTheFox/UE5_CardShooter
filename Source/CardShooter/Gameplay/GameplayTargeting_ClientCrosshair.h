#pragma once

#include "Abilities/GameplayAbilityTargetActor.h"

#include "GameplayTargeting_ClientCrosshair.generated.h"

UCLASS()
class AGameplayTageting_ClientCrosshair : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	AGameplayTageting_ClientCrosshair();

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual bool IsConfirmTargetingAllowed() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting")
	float TraceRange = 10000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Targeting")
	bool bTraceComplex = false;

	FHitResult PerformCrosshairTrace() const;
	APlayerController* GetOwningPlayerController() const;
};