//#include "AsyncAction_DelayedExecuteCard.h"
//
//
//UDelayedCardExecution* UDelayedCardExecution::DelayedCardExecution(float InDelay, UCardEffectFragment_Execution* InExecution, FCardExecution_AdditionalData InParams)
//{
//	UDelayedCardExecution* NewNode = NewObject<UDelayedCardExecution>();
//	NewNode->Delay = InDelay;
//	NewNode->Execution = InExecution;
//	NewNode->Params = InParams;
//
//	return NewNode;
//}
//
//void UDelayedCardExecution::Activate()
//{
//	FTimerHandle Handle;
//	GetWorld()->GetTimerManager().SetTimer(
//		Handle,
//		[Execution, Params]()
//		{
//			Execution->ExecuteCardEffect(ExecutingPawn, Params);
//		},
//		Delay
//		);
//}
//
