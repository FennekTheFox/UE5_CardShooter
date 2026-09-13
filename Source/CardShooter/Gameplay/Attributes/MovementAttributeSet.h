#pragma once

#include "CoreMinimal.h"
#include "ProjectAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MovementAttributeSet.generated.h"

UCLASS()
class UMovementAttributeSet : public UProjectAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MoveSpeed)
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, RotationRate)
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, JumpHeight)
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, Acceleration)
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, BrakingDeceleration)
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, AirControl)
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, GravityScale)

	UMovementAttributeSet();

	virtual void PreAttributeChange(
		const FGameplayAttribute& Attribute,
		float& NewValue) override;

	virtual void PostGameplayEffectExecute(
		const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_RotationRate(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_JumpHeight(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Acceleration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BrakingDeceleration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_AirControl(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_GravityScale(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MoveSpeed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RotationRate, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData RotationRate;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_JumpHeight, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData JumpHeight;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Acceleration, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Acceleration;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BrakingDeceleration, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BrakingDeceleration;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AirControl, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AirControl;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_GravityScale, Category = "Movement", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData GravityScale;
};