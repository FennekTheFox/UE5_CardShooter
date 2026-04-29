// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarwolfEMissionSubObjWidget.generated.h"

class URichTextBlock;

/**
 * A widget containing a sub-objective information for the player.
 */
UCLASS()
class WARWOLFMISSIONSYSTEM_API UWarwolfExampleMissionSubObjWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*
	 * @brief Get the guid of this sub-objective widget.
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
	 * @brief Set the text displayed on the widget.
	 * @param InSubObjText - The FGuid we want to assign to this widget. 
	 */
	UFUNCTION()
	void SetSubObjectiveText(const FText InSubObjText);

	// @brief Play the appear anim. 
	UFUNCTION()
	void PlayAppearanceAnim();

	// @brief Play finished animation and remove this sub-objective widget. 
	UFUNCTION()
	void HideSubObjective();

	// @brief Update text with an int count. Only should be called from the int count state tree task. 
	UFUNCTION()
	void UpdateCount(const int32 InCurrentCount, const int32 InTotalCount) const;

	// @brief Update text with a time amount. Only should be called from the timer state tree task. 
	UFUNCTION()
	void UpdateTime(const float InCurrentTime, const float InTotalTime) const;
	

	// Widgets

	// @brief Text we want to display the sub-objective as.
	UPROPERTY(Transient, meta=(BindWidget))
	TObjectPtr<URichTextBlock> SubObjText;

	// @brief The animation which plays when showing the sub-objective for the first time.
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SubObjAppearAnim;

	// @brief The animation which plays when hiding the sub-objective.
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SubObjFinishedAnim;

	// ~Widgets

private:

	// @brief The Guid assigned to this widget. This makes it easier to find.
	UPROPERTY(Transient)
	FGuid WidgetGuid = FGuid();

	// @brief The text we're meant to display here. Cache it as we may need to update the count.
	UPROPERTY(Transient)
	FText SubObjectiveText = FText();

	FWidgetAnimationDynamicEvent FinishAnimDelegate;
	
};
