#pragma once

#include "Subsystems/GameInstanceSubsystem.h"

#include "CardShooterSaveGameSubsystem.generated.h"


class UCardShooterSaveData;

UCLASS(BlueprintType)
class UCardShooterSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void GetLocalSaveGameList(TArray<UCardShooterSaveData*>& OutSaveGames);

	UFUNCTION()
	bool SaveGame(UCardShooterSaveData* Data);

	UFUNCTION(BlueprintPure)
	FString GetPlayerSaveGameDir();
};