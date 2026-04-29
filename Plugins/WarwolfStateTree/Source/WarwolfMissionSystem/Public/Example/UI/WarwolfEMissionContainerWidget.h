// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarwolfMissionTypes.h"
#include "WarwolfEMissionContainerWidget.generated.h"

class UVerticalBox;
class UWarwolfExampleMissionObjectiveWidget;
class UWarwolfExampleMissionSubObjWidget;

/**
 * 
 */
UCLASS()
class WARWOLFMISSIONSYSTEM_API UWarwolfExampleMissionContainerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*
	 * @brief Get an objective widget with the provided guid.
	 * @param InGuid - The FGuid of the widget we want to find.
	 * @return The found widget.
	 * @warning May return nullptr.
	 */
	UFUNCTION()
	UWarwolfExampleMissionObjectiveWidget* GetObjectiveWithGuid(const FGuid InGuid) const;

	/*
	 * @brief Get a sub-objective widget with the provided guid.
	 * @param ObjectiveGuid - The FGuid of the objective widget the sub-objective is expected to be in.
	 * @param SubObjectiveGuid - The FGuid of the sub-objective widget we want to find.
	 * @return The found widget.
	 * @warning May return nullptr.
	 */
	UFUNCTION()
	UWarwolfExampleMissionSubObjWidget* GetSubObjectiveWithGuid(const FGuid ObjectiveGuid, const FGuid SubObjectiveGuid) const;

	
	// Widgets

	// @brief Widget that controls the toast UI.
	UPROPERTY(BlueprintReadOnly, Category="WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<UVerticalBox> ObjectiveList;

	// ~Widgets
};
