#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MovementAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UMovementAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UMovementAttributeSet();

	virtual void PreAttributeChange(
		const FGameplayAttribute& Attribute,
		float& NewValue) override;

	virtual void PostGameplayEffectExecute(
		const FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	/*
	 * CharacterMovement->MaxWalkSpeed
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category = "Movement")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MoveSpeed)

		UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);


	/*
	 * CharacterMovement->RotationRate.Yaw
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RotationRate, Category = "Movement")
	FGameplayAttributeData RotationRate;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, RotationRate)

		UFUNCTION()
	void OnRep_RotationRate(const FGameplayAttributeData& OldValue);


	/*
	 * CharacterMovement->JumpZVelocity
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_JumpHeight, Category = "Movement")
	FGameplayAttributeData JumpHeight;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, JumpHeight)

		UFUNCTION()
	void OnRep_JumpHeight(const FGameplayAttributeData& OldValue);


	/*
	 * CharacterMovement->MaxAcceleration
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Acceleration, Category = "Movement")
	FGameplayAttributeData Acceleration;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, Acceleration)

		UFUNCTION()
	void OnRep_Acceleration(const FGameplayAttributeData& OldValue);


	/*
	 * CharacterMovement->BrakingDecelerationWalking
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BrakingDeceleration, Category = "Movement")
	FGameplayAttributeData BrakingDeceleration;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, BrakingDeceleration)

		UFUNCTION()
	void OnRep_BrakingDeceleration(const FGameplayAttributeData& OldValue);


	/*
	 * CharacterMovement->AirControl
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AirControl, Category = "Movement")
	FGameplayAttributeData AirControl;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, AirControl)

		UFUNCTION()
	void OnRep_AirControl(const FGameplayAttributeData& OldValue);


	/*
	 * CharacterMovement->GravityScale
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_GravityScale, Category = "Movement")
	FGameplayAttributeData GravityScale;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, GravityScale)

		UFUNCTION()
	void OnRep_GravityScale(const FGameplayAttributeData& OldValue);
};