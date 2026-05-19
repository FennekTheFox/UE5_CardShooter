#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AttributeDrivenMovementComponent.generated.h"

class UAbilitySystemComponent;

UCLASS()
class UAttributeDrivenCharacterMovementComponent
	: public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UAttributeDrivenCharacterMovementComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Movement Attributes")
	void InitializeFromAbilitySystem();

	UFUNCTION(BlueprintCallable, Category = "Movement Attributes")
	void ApplyAllMovementAttributes();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

private:
	FDelegateHandle MoveSpeedHandle;
	FDelegateHandle RotationRateHandle;
	FDelegateHandle JumpHeightHandle;
	FDelegateHandle AccelerationHandle;
	FDelegateHandle BrakingDecelerationHandle;
	FDelegateHandle AirControlHandle;
	FDelegateHandle GravityScaleHandle;

	void BindAttributeDelegates();
	void UnbindAttributeDelegates();

	void HandleMoveSpeedChanged(const struct FOnAttributeChangeData& Data);
	void HandleRotationRateChanged(const struct FOnAttributeChangeData& Data);
	void HandleJumpHeightChanged(const struct FOnAttributeChangeData& Data);
	void HandleAccelerationChanged(const struct FOnAttributeChangeData& Data);
	void HandleBrakingDecelerationChanged(const struct FOnAttributeChangeData& Data);
	void HandleAirControlChanged(const struct FOnAttributeChangeData& Data);
	void HandleGravityScaleChanged(const struct FOnAttributeChangeData& Data);

	void ApplyMoveSpeed(float NewValue);
	void ApplyRotationRate(float NewValue);
	void ApplyJumpHeight(float NewValue);
	void ApplyAcceleration(float NewValue);
	void ApplyBrakingDeceleration(float NewValue);
	void ApplyAirControl(float NewValue);
	void ApplyGravityScale(float NewValue);
};