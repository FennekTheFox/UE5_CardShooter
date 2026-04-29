// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponentSchema.h"
//
#include "WarwolfMissionDataAsset.h"
//
#include "WarwolfMissionStComponentSchema.generated.h"

/**
 * 
 */
UCLASS()
class WARWOLFMISSIONSYSTEM_API UWarwolfMissionStComponentSchema : public UStateTreeComponentSchema
{
	GENERATED_BODY()

	virtual bool IsExternalItemAllowed(const UStruct& InStruct) const override
	{
		if (InStruct.IsChildOf(UWarwolfMissionDataAsset::StaticClass()))
		{
			return true;
		}
		
		return Super::IsExternalItemAllowed(InStruct);
	}
};
