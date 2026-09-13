#pragma once

#include "AbilitySystemComponent.h"
#include "ProjectAttributeSet.h"

#include "HealthAttributeSet.generated.h"

UCLASS()
class UHealthAttributeSet : public UProjectAttributeSet
{
	GENERATED_BODY()

public:
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, IncomingDamage)
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, TotalCurrentShieldAmount)

protected:
	//virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	//virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	//Handles Attribute Replication for Health on the client side
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	//Handles Attribute Replication for MaxHealth on the client side
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	//Handles Attribute Replication for MaxHealth on the client side
	UFUNCTION()
	void OnRep_TotalCurrentShieldAmount(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData IncomingDamage;


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_TotalCurrentShieldAmount, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData TotalCurrentShieldAmount;
};