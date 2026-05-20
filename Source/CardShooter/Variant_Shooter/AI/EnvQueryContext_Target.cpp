// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Shooter/AI/EnvQueryContext_Target.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "ShooterAIController.h"

void UEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	AAIController* Controller = Cast<AAIController>(QueryInstance.Owner);

	if (!Controller)
	{
		if (APawn* Pawn = Cast<APawn>(QueryInstance.Owner))
		{
			Controller = Cast<AAIController>(Pawn->GetController());
		}
	}

	// get the controller from the query instance
	if (Controller)
	{
		// ensure the target is valid
		if (IsValid(Controller->GetFocusActor()))
		{
			// add the controller's target actor to the context
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Controller->GetFocusActor());

		} else {

			// if for any reason there's no target, default to the controller
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Controller);
		}
	}

}
