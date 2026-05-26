#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "PoseAssetActionUtility.generated.h"

UCLASS()
class CARDSHOOTER_API UPoseAssetActionsUtility : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor, Category = "Pose Assets")
	void RefreshPoseAssetsFromSourceAnimation();
};