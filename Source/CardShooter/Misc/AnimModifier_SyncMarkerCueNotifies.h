#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimModifier_SyncMarkerCueNotifies.generated.h"

USTRUCT(BlueprintType)
struct FSyncMarkerCueNotifyMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Marker")
	FName SyncMarkerLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Marker")
	FName FootDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	TSubclassOf<UAnimNotify> NotifyClass;
};

UCLASS()
class CARDSHOOTER_API UAnimModifier_SyncMarkerCueNotifies : public UAnimationModifier
{
	GENERATED_BODY()

public:
	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	virtual void OnRevert_Implementation(UAnimSequence* AnimationSequence) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Sync Marker Cue Notifies")
	TArray<FSyncMarkerCueNotifyMapping> MarkerMappings;

	UPROPERTY(EditAnywhere, Category = "Sync Marker Cue Notifies")
	FName NotifyTrackName = TEXT("GameplayCues.Footsteps");

	UPROPERTY(EditAnywhere, Category = "Sync Marker Cue Notifies")
	bool bClearExistingGeneratedNotifies = true;

	UPROPERTY(EditAnywhere, Category = "Sync Marker Cue Notifies")
	float DuplicateTimeTolerance = 0.001f;

	UPROPERTY(EditAnywhere, Category = "Sync Marker Cue Notifies")
	FString GeneratedNotifyNamePrefix = TEXT("GeneratedCue_");

private:
	const FSyncMarkerCueNotifyMapping* FindMapping(FName SyncMarkerLabel) const;
	void RemoveGeneratedNotifies(UAnimSequence* AnimationSequence) const;
	int32 FindOrAddNotifyTrack(UAnimSequence* AnimationSequence) const;
};