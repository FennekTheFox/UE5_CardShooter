//#pragma once
//
//#include "ProjectAbilitySystemComponent.h"
//
//#include "CardShooterAbilitySystemComponent.generated.h"
//
//UCLASS(BlueprintType, Blueprintable)
//class UCardShooterAbilitySystemComponent : public UProjectAbilitySystemComponent
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(EditDefaultsOnly)
//	FGameplayAbilitySpecDef PrimaryTargetDataBridgeSpecDef;
//
//	UPROPERTY(EditDefaultsOnly)
//	FGameplayAbilitySpecDef SecondaryTargetDataBridgeSpecDef;
//
//	UFUNCTION(BlueprintCallable)
//	void Request_SetTargetData(FGameplayAbilityTargetDataHandle Data);
//
//
//	void OnRegister() override;
//	void OnUnregister() override;
//
//private:
//	FGameplayAbilitySpecHandle PrimaryTargetingBridgeHandle;
//	FGameplayAbilitySpecHandle SecondaryTargetingBridgeHandle;
//};