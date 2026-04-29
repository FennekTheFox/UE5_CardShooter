// Copyright (c) 2024. Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
//
class UWarwolfMissionDataAsset;
//
#include "WarwolfMissionTypes.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWarwolfMissionSystem, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWarwolfMissionUI, Log, All);

USTRUCT(BlueprintType)
struct FWarwolfMissionDataRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	
public:

	FWarwolfMissionDataRow()
			: MissionData(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WarwolfMissionSystem")
	TObjectPtr<UWarwolfMissionDataAsset> MissionData;
	
};

UENUM(BlueprintType)
enum class EWarwolfMissionType : uint8
{
	Mission UMETA(DisplayName = "Mission")
	, Primary UMETA(DisplayName = "Primary Objective")
	, Optional UMETA(DisplayName = "Optional Objective")
};

UENUM(BlueprintType)
enum class EWarwolfMissionStatus : uint8
{
	NotFinished UMETA(DisplayName = "Not Finished")
	, Success UMETA(DisplayName = "Successfully Completed")
	, Fail UMETA(DisplayName = "Failed")
};

UENUM(BlueprintType)
enum class EWarwolfMissionToastType : uint8
{
	MissionStart UMETA(DisplayName = "Mission Start")
	, MissionUpdate UMETA(DisplayName = "Mission Updated")
	, MissionSuccess UMETA(DisplayName = "Mission Completed")
	, MissionFailure UMETA(DisplayName = "Mission Failed")
	, ObjectiveStart UMETA(DisplayName = "Objective Start")
	, ObjectiveUpdate UMETA(DisplayName = "Objective Updated")
	, ObjectiveSuccess UMETA(DisplayName = "Objective Completed")
	, ObjectiveFailure UMETA(DisplayName = "Objectiven Failed")
};

UCLASS()
class WARWOLFMISSIONSYSTEM_API UWarwolfMissionTypes : public UObject
{
	GENERATED_BODY()
};
