#include "MovementAttributeSet.h"

#include "Net/UnrealNetwork.h"

UMovementAttributeSet::UMovementAttributeSet()
{
	InitMoveSpeed(600.f);
	InitRotationRate(540.f);
	InitJumpHeight(420.f);
	InitAcceleration(2048.f);
	InitBrakingDeceleration(2048.f);
	InitAirControl(0.2f);
	InitGravityScale(1.f);
}

void UMovementAttributeSet::PreAttributeChange(
	const FGameplayAttribute& Attribute,
	float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMoveSpeedAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 5000.f);
	}
	else if (Attribute == GetRotationRateAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 2000.f);
	}
	else if (Attribute == GetJumpHeightAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 3000.f);
	}
	else if (Attribute == GetAccelerationAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 10000.f);
	}
	else if (Attribute == GetBrakingDecelerationAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 10000.f);
	}
	else if (Attribute == GetAirControlAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, 1.f);
	}
	else if (Attribute == GetGravityScaleAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.1f, 10.f);
	}
}

void UMovementAttributeSet::PostGameplayEffectExecute(
	const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UMovementAttributeSet::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(
		UMovementAttributeSet,
		MoveSpeed,
		COND_None,
		REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(
		UMovementAttributeSet,
		RotationRate,
		COND_None,
		REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(
		UMovementAttributeSet,
		JumpHeight,
		COND_None,
		REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(
		UMovementAttributeSet,
		Acceleration,
		COND_None,
		REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(
		UMovementAttributeSet,
		BrakingDeceleration,
		COND_None,
		REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(
		UMovementAttributeSet,
		AirControl,
		COND_None,
		REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(
		UMovementAttributeSet,
		GravityScale,
		COND_None,
		REPNOTIFY_Always);
}

void UMovementAttributeSet::OnRep_MoveSpeed(
	const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(
		UMovementAttributeSet,
		MoveSpeed,
		OldValue);
}

void UMovementAttributeSet::OnRep_RotationRate(
	const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(
		UMovementAttributeSet,
		RotationRate,
		OldValue);
}

void UMovementAttributeSet::OnRep_JumpHeight(
	const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(
		UMovementAttributeSet,
		JumpHeight,
		OldValue);
}

void UMovementAttributeSet::OnRep_Acceleration(
	const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(
		UMovementAttributeSet,
		Acceleration,
		OldValue);
}

void UMovementAttributeSet::OnRep_BrakingDeceleration(
	const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(
		UMovementAttributeSet,
		BrakingDeceleration,
		OldValue);
}

void UMovementAttributeSet::OnRep_AirControl(
	const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(
		UMovementAttributeSet,
		AirControl,
		OldValue);
}

void UMovementAttributeSet::OnRep_GravityScale(
	const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(
		UMovementAttributeSet,
		GravityScale,
		OldValue);
}