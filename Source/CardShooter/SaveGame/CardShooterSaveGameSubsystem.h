#pragma once

#include "Subsystems/GameInstanceSubsystem.h"

#include "CardShooterSaveGameSubsystem.generated.h"


class UCardShooterSaveData;

UCLASS(BlueprintType)
class UCardShooterSaveGameSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	void GetLocalSaveGameList(TArray<UCardShooterSaveData*>& OutSaveGames);

	UFUNCTION(BlueprintPure)
	void GetLocalPlayerSaveGameName(TArray<FString>& OutSaveGames);

	UFUNCTION(BlueprintCallable)
	bool SaveGame();

	UFUNCTION(BlueprintPure)
	FString GetPlayerSaveGameDir();

	UFUNCTION(BlueprintCallable)
	void SetPlayerLoadedSaveData(UCardShooterSaveData* Data);

	UFUNCTION(BlueprintPure)
	UCardShooterSaveData* GetLocalPlayerSaveData();

	UFUNCTION(BlueprintCallable)
	bool GetLastEditedSave(FString& OutProfileName);

private:
	UPROPERTY()
	TObjectPtr<UCardShooterSaveData> LocalPlayerSaveData;
};