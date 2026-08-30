

#pragma once

#include "CoreMinimal.h"
#include "ProjectPlayerController.h"

#include "CardShooterPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FInputDeviceTypeChanged, bool)

/**
 *  Simple first person Player Controller
 *  Manages the input mapping context.
 *  Overrides the Player Camera Manager class.
 */
UCLASS(abstract, config="Game")
class CARDSHOOTER_API ACardShooterPlayerController : public AProjectPlayerController
{
	GENERATED_BODY()
	
public:

	/** Constructor */

	ACardShooterPlayerController();
	bool InputKey(const FInputKeyEventArgs& Params) override;
	bool bLastInputWasGamepad = false;
	FInputDeviceTypeChanged OnDeviceTypeChanged;

protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	UPROPERTY()
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** If true, the player will use UMG touch controls even if not playing on mobile platforms */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Touch Controls")
	bool bForceTouchControls = false;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	/** Returns true if the player should use UMG touch controls */
	bool ShouldUseTouchControls() const;

	virtual void UpdateRotation (float DeltaTime) override;

	// Converts a rotation from world space to gravity relative space.
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityRelativeRotation(FRotator Rotation, FVector GravityDirection);

	// Converts a rotation from gravity relative space to world space.
	UFUNCTION(BlueprintPure)
	static FRotator GetGravityWorldRotation(FRotator Rotation, FVector GravityDirection);

	UPROPERTY(BlueprintReadOnly)
	FVector LastFrameGravity = FVector::DownVector;
};
