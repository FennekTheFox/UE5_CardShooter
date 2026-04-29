// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarwolfMissionTypes.h"
#include "Blueprint/UserWidget.h"
#include "WarwolfEMissionObjectiveWidget.generated.h"

class UBorder;
class URichTextBlock;
class UVerticalBox;
class UWarwolfExampleMissionSubObjWidget;

/**
 * 
 */
UCLASS()
class WARWOLFMISSIONSYSTEM_API UWarwolfExampleMissionObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	/*
	* @brief Get the guid of this objective widget.
	* @return The FGuid assigned to this widget. 
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "WarwolfMissionUI")
	FGuid GetGuid() const { return WidgetGuid; }

	/*
	 * @brief Set the guid of this sub-objective widget.
	 * @param InGuid - The FGuid we want to assign to this widget. 
	 */
	UFUNCTION()
	void SetGuid(const FGuid InGuid) { WidgetGuid = InGuid; }

	/*
	 * @brief Set the objective name on the widget.
	 * @param InObjectiveText - The text we want to display the objective as.
	 */
	UFUNCTION()
	void SetObjectiveName(const FText InObjectiveText);

	// @brief Hide this objective widget. 
	UFUNCTION()
	void HideObjective();

	/*
	 * @brief Set the type of objective this is.
	 * @param InObjectiveType - The type of objective this is, i.e. primary or secondary.
	 */
	UFUNCTION()
	void SetObjectiveType(const EWarwolfMissionType InObjectiveType);

	// @brief Get all of the sub-objective widgets this objective is displaying.
	UFUNCTION()
	TArray<UWarwolfExampleMissionSubObjWidget*> GetAllSubObjectives();

	
	// Widgets

	// @brief Text we want to display the objective as.
	UPROPERTY(BlueprintReadOnly, Category = "WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<URichTextBlock> ObjectiveNameText;

	// @brief Container for all the sub-objectives for this objective.
	UPROPERTY(BlueprintReadOnly, Category = "WarwolfMissionSystem", Transient, meta=(BindWidget))
	TObjectPtr<UVerticalBox> SubObjContainer;

	// @brief Border that shows the background colour.
	UPROPERTY(BlueprintReadOnly, Category="WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<UBorder> ObjMark;

	// ~Widgets
	

private:

	// @brief The Guid assigned to this widget. This makes it easier to find.
	UPROPERTY()
	FGuid WidgetGuid = FGuid();
	
};
