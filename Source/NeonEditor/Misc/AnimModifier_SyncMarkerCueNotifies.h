// AnimModifier_SyncMarkerCueNotifies.h

#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "AnimModifier_SyncMarkerCueNotifies.generated.h"

class UAnimNotify;

USTRUCT(BlueprintType)
struct FSyncMarkerCueNotifyMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Marker Cue")
	FName SyncMarkerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Marker Cue")
	TSubclassOf<UAnimNotify> NotifyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Marker Cue")
	float TimeOffset = 0.0f;
};

UCLASS()
class UAnimModifier_SyncMarkerCueNotifies : public UAnimationModifier
{
	GENERATED_BODY()

public:
	UAnimModifier_SyncMarkerCueNotifies();

protected:
	virtual void OnApply_Implementation(UAnimSequence* InAnimation) override;
	virtual void OnRevert_Implementation(UAnimSequence* InAnimation) override;

private:
	void ValidateNotifyTrack(UAnimSequence* InAnimation);
	void PrepareNotifyTrack(UAnimSequence* InAnimation);

	UPROPERTY(EditAnywhere, Category = "Sync Marker Cue")
	FName NotifyTrackName;

	UPROPERTY(EditAnywhere, Category = "Sync Marker Cue")
	TArray<FSyncMarkerCueNotifyMapping> SyncMarkerNotifyMappings;

	UPROPERTY(EditAnywhere, Category = "Sync Marker Cue")
	bool bShouldRemovePreExistingNotifiesOnTrack;

	UPROPERTY(Transient)
	bool bGeneratedNotifyTrack;
};