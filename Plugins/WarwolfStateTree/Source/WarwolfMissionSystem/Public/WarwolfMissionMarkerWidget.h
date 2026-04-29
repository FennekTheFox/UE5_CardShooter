// Copyright (c) 2025 Michael Greenard. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "WarwolfMissionTypes.h"
#include "Blueprint/UserWidget.h"
#include "WarwolfMissionMarkerWidget.generated.h"

class UVerticalBox;
class URichTextBlock;
class UCanvasPanel;

/**
 * Native base class for widget that displays when a mission marker is active. Should always have a blueprint layer.
 */
UCLASS()
class WARWOLFMISSIONSYSTEM_API UWarwolfMissionMarkerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	/**
	* Converts a world location to screen position for HUD drawing. This differs from the results of FSceneView::WorldToScreen in that it returns a position along the edge of the screen for offscreen locations
	* Thanks OP and ksztanan https://forums.unrealengine.com/t/easy-offscreen-indicator-blueprint-node/21342/52
	*
	* @param InLocation - The world space location to be converted to screen space
	* @param EdgePercent - How close to the edge of the screen, 1.0 = at edge, 0.0 = at center of screen. .9 or .95 is usually desirable
	* @param ViewportCenterLoc - for offsetting center of the screen, leave at (0.5, 0.5) for no offset
	* @return OutScreenPosition - the screen coordinates for HUD drawing
	* @return OutRotationAngleDegrees - The angle to rotate a hud element if you want it pointing toward the offscreen indicator, 0° if onscreen
	* @return bIsOnScreen - True if the specified location is in the camera view (may be obstructed)
	*/
	UFUNCTION()
	void FindScreenEdgeLocationForWorldLocation(const FVector& InLocation, const float EdgePercent, const FVector2D ViewportCenterLoc, FVector2D& OutScreenPosition, float& OutRotationAngleDegrees, bool &bIsOnScreen) const;

	/*
	 * @brief Update the name of the marker.
	 * @param InName - Name this marker will display.
	*/
	UFUNCTION()
	void UpdateMarkerName(const FText InName);

	/*
	 * @brief Update distance text based on marker distance to player.
	 * @param InDistance - Distance to display on the marker.
	*/ 
	UFUNCTION()
	void UpdateMarkerDistance(const FText InDistance);

	/*
	 * @brief Update location of the owning marker as it may be attached to something.
	 * @param InLocation - World location of the owning marker.
	*/ 
	UFUNCTION()
	void UpdateMarkerLocation(const FVector InLocation);

	/*
	 * @brief Update colour of the marker based on the type of objective it's for.
	 * @param InMissionType - Type of mission this marker is used for.
	*/ 
	UFUNCTION(BlueprintNativeEvent)
	void UpdateMarkerType(const EWarwolfMissionType InMissionType);

	// @brief At what percent does the marker stop at the edges of the screen? Best left at default for consistency.
	UPROPERTY(EditDefaultsOnly, Category = "WarwolfMissionSystem", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float MarkerEdgePercent = 0.9f;

	// @brief World location of the owning marker. 
	UPROPERTY()
	FVector OwningMarkerLocation = FVector();

	// Widgets

	// @brief Container for the widget's marker visuals. Used to move the marker around in screen space.
	UPROPERTY(BlueprintReadWrite, Category="WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<UVerticalBox> WidgetContainer;

	// @brief Name of the marker, e.g. a location name.
	UPROPERTY(BlueprintReadWrite, Category="WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<URichTextBlock> MarkerText;

	// @brief Distance of the marker from the player.
	UPROPERTY(BlueprintReadWrite, Category="WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<URichTextBlock> MarkerDistText;

	// @brief Distance of the marker from the player.
	UPROPERTY(BlueprintReadWrite, Category="WarwolfMissionSystem", meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	// ~Widgets

};
