// Copyright (c) 2025 Michael Greenard. All rights reserved.


#include "WarwolfMissionMarkerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "WarwolfMissionTypes.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/RichTextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Engine/GameViewportClient.h"
#include "UnrealClient.h"

void UWarwolfMissionMarkerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!WidgetContainer) return;

	FVector2D ScreenPosition;
	float RotationAngleDegrees;
	bool bIsOnScreen;

	FindScreenEdgeLocationForWorldLocation(	OwningMarkerLocation
											, MarkerEdgePercent
											, FVector2d(0.5f, 0.5f)
											, ScreenPosition
											, RotationAngleDegrees
											, bIsOnScreen);

	// Convert to local space of the Canvas Panel
	FVector2D LocalCoordinate;
	USlateBlueprintLibrary::ScreenToWidgetLocal(this, CanvasPanel->GetCachedGeometry(), ScreenPosition, LocalCoordinate);

	// Update position in Canvas panel with the size offset
	UCanvasPanelSlot* WidgetAsSlot = Cast<UCanvasPanelSlot>(WidgetContainer->Slot);
	if(!WidgetAsSlot)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s WidgetAsSlot is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}
	
	const float SizeOffsetX = WidgetAsSlot->GetSize().X / 2.f; 
	const float SizeOffsetY = WidgetAsSlot->GetSize().Y / 2.f; 
	WidgetAsSlot->SetPosition(LocalCoordinate + FVector2D(-SizeOffsetX, -SizeOffsetY));

}

void UWarwolfMissionMarkerWidget::FindScreenEdgeLocationForWorldLocation(const FVector& InLocation, const float EdgePercent, const FVector2D ViewportCenterLoc, FVector2D& OutScreenPosition, float& OutRotationAngleDegrees, bool& bIsOnScreen) const
{
	bIsOnScreen = false;
	OutRotationAngleDegrees = 0.f;
	FVector2D ScreenPosition = FVector2D();

	if(!GEngine)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s GEngine is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const FIntPoint ViewportSizeInt = FIntPoint(GEngine->GameViewport->Viewport->GetSizeXY());
	const FVector2D ViewportSize = FVector2D(ViewportSizeInt.X, ViewportSizeInt.Y);
	const FVector2D ViewportCenter = FVector2D( ViewportSize.X * ViewportCenterLoc.X, ViewportSize.Y * ViewportCenterLoc.Y );

	const UWorld* World = GetWorld();
	if(!World)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s World is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0 );
	if(!PlayerController)
	{
		UE_LOG(LogWarwolfMissionSystem, Error, TEXT("%s PlayerController is not valid. Returning."), *FString(__FUNCTION__));
		return;
	}

	FVector CameraLoc;
	FRotator CameraRot;

	PlayerController->GetPlayerViewPoint(CameraLoc, CameraRot);

	const FVector CameraToLoc = InLocation - CameraLoc;
	const FVector Forward = CameraRot.Vector();
	const FVector Offset = CameraToLoc.GetSafeNormal();

	const float DotProduct = FVector::DotProduct( Forward, Offset);
	const bool bLocationIsBehindCamera = (DotProduct < 0);

	if(bLocationIsBehindCamera)
	{
		const FVector Inverted = CameraToLoc * -1.f;
		const FVector NewInLocation = CameraLoc + Inverted;

		PlayerController->ProjectWorldLocationToScreen(NewInLocation, ScreenPosition);

		ScreenPosition.X = ViewportSize.X - ScreenPosition.X;
		ScreenPosition.Y = ViewportSize.Y - ScreenPosition.Y;
	}
	else
	{
		PlayerController->ProjectWorldLocationToScreen(InLocation, ScreenPosition);
	}

	// Check to see if it's on screen. If it is, ProjectWorldLocationToScreen is all we need, return it.
	if(	ScreenPosition.X >= 0.f
		&& ScreenPosition.X <= ViewportSize.X
		&& ScreenPosition.Y >= 0.f
		&& ScreenPosition.Y <= ViewportSize.Y
		&& !bLocationIsBehindCamera)
	{
		OutScreenPosition = ScreenPosition;
		bIsOnScreen = true;
		return;
	}

	ScreenPosition -= ViewportCenter;

	float AngleRadians = FMath::Atan2(ScreenPosition.Y, ScreenPosition.X);
	AngleRadians -= FMath::DegreesToRadians(90.f);

	OutRotationAngleDegrees = FMath::RadiansToDegrees(AngleRadians) + 180.f;

	const float Cos = cosf(AngleRadians);
	const float Sin = -sinf(AngleRadians);

	ScreenPosition = FVector2D(ViewportCenter.X + (Sin * 150.f), ViewportCenter.Y + Cos * 150.f);

	const float M = Cos / Sin;

	const FVector2D ScreenBounds = ViewportCenter * EdgePercent;

	if(Cos > 0)
	{
		ScreenPosition = FVector2D(ScreenBounds.Y / M, ScreenBounds.Y);
	}
	else
	{
		ScreenPosition = FVector2D(-ScreenBounds.Y / M, -ScreenBounds.Y);
	}

	if( ScreenPosition.X > ScreenBounds.X )
	{
		ScreenPosition = FVector2D(ScreenBounds.X, ScreenBounds.X * M);
	}
	else if(ScreenPosition.X < -ScreenBounds.X)
	{
		ScreenPosition = FVector2D(-ScreenBounds.X, -ScreenBounds.X * M);
	}

	ScreenPosition += ViewportCenter;

	OutScreenPosition = ScreenPosition; // / UWidgetLayoutLibrary::GetViewportScale(this);
}

void UWarwolfMissionMarkerWidget::UpdateMarkerName(const FText InName)
{
	if(!MarkerText) return;

	MarkerText->SetText(InName);
}

void UWarwolfMissionMarkerWidget::UpdateMarkerDistance(const FText InDistance)
{
	if(!MarkerDistText) return;

	MarkerDistText->SetText(InDistance);
}

void UWarwolfMissionMarkerWidget::UpdateMarkerLocation(const FVector InLocation)
{
	OwningMarkerLocation = InLocation;
}

void UWarwolfMissionMarkerWidget::UpdateMarkerType_Implementation(const EWarwolfMissionType InMissionType)
{
	
}
