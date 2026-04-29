// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WarwolfMissionUiInterface.generated.h"

UINTERFACE(MinimalAPI)
class UWarwolfMissionUiInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface is for interacting between UI and the mission system.
 */
class WARWOLFMISSIONSYSTEM_API IWarwolfMissionUiInterface
{
	GENERATED_BODY()

public:

	/*
	 * @brief Show the player that an objective has started.
	 * @param InMissionName - Name of the mission to show.
	 * @param InMissionType - Type of mission we're displaying.
	 * @param InMissionIndex - Index of the mission we're updating.
	 * @param InToastType - The type of toast we want to display
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarwolfMissionSystem")
	void ToastMission(const FText& InMissionName, const EWarwolfMissionType InMissionType, const EWarwolfMissionToastType InToastType);

	/*
	 * @brief Show the player an objective
	 * @param InObjText - Text to show on the obj widget.
	 * @param InGuid - The guid we want to assign the objective widget.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarwolfMissionSystem")
	void ShowObjective(const FText& InObjText, const FGuid InGuid, const EWarwolfMissionType InMissionType);

	/*
	 * @brief Hide the given objective.
	 * @param InGuid - The guid of the obj widget we want to hide.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarwolfMissionSystem")
	void HideObjective(const FGuid InGuid);

	/*
	 * @brief Show the player a sub-objective
	 * @param InSubObjText - Text to show on the sub-obj widget.
	 * @param InObjGuid - Guid of the objective widget we want to add to.
	 * @param InGuid - The guid we want to assign the sub-objective widget.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarwolfMissionSystem")
	void ShowSubObjective(const FText& InSubObjText, const FGuid InObjGuid, const FGuid InGuid);

	/*
	 * @brief Hide given sub-objective.
	 * @param InObjGuid - Guid of the objective widget we're trying to remove this sub-obj from.
	 * @param InGuid - The guid of the sub-obj widget we want to hide.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarwolfMissionSystem")
	void HideSubObjective(const FGuid InObjGuid, const FGuid InGuid);

	/*
	 * @brief Update a sub-objective with the passed guid with counted ints.
	 * @param InObjGuid - Guid of the objective widget containing the sub-obj.
	 * @param InGuid - The guid of the sub-obj widget we want to update.
	 * @param InCurrentCount - Currently counted int
	 * @param InTotalCount - Total count for the int.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarwolfMissionSystem")
	void UpdateSubObjective_IntCount(const FGuid InObjGuid, const FGuid InGuid, const int32 InCurrentCount, const int32 InTotalCount);

	/*
	 * @brief Update a sub-objective with the passed guid with timer time.
	 * @param InObjGuid - Guid of the objective widget containing the sub-obj.
	 * @param InGuid - The guid of the sub-obj widget we want to update.
	 * @param InCurrentTime - Currently counted time
	 * @param InTotalTime - Total time.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "WarwolfMissionSystem")
	void UpdateSubObjective_Timer(const FGuid InObjGuid, const FGuid InGuid, const float InCurrentTime, const float InTotalTime);
	
};
