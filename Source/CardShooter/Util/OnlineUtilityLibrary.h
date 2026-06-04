//#pragma once
//
//#include "CoreMinimal.h"
//#include "Kismet/BlueprintAsyncActionBase.h"
//#include "Cards/Fragments/CardAttributeFragment_Execution.h"
//#include "AsyncAction_DelayedExecuteCard.generated.h"
//
//UCLASS()
//class UDelayedCardExecution : public UBlueprintAsyncActionBase
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite)
//	TObjectPtr<UCardEffectFragment_Execution> Execution;
//
//	UPROPERTY(BlueprintReadWrite)
//	TWeakObjectPtr<APawn> Execution;
//
//	UPROPERTY(BlueprintReadWrite)
//	FCardExecution_AdditionalData Params;
//
//	UPROPERTY(BlueprintReadWrite)
//	float Delay = 1.f;
//
//
//	virtual void Activate() override;
//};