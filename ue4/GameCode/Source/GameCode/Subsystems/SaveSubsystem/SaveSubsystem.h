#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveData.h"
#include "SaveSubsystem.generated.h"


UCLASS()
class GAMECODE_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	const FGameSaveData& GetGameSaveData() const;

	UFUNCTION(BlueprintCallable, Category = "Save Subsystem")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save Subsystem")
	void LoadLastGame();

	UFUNCTION(BlueprintCallable, Category = "Save Subsystem")
	void LoadGame(int32 SaveId);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void SerializeLevel(const ULevel* Level, const ULevelStreaming* StreamingLevel = nullptr);
	void DeserializeLevel(ULevel* Level, const ULevelStreaming* StreamingLevel = nullptr);

private:
	void SerializeGame();
	void DeserializeGame();
	void WriteSaveToFile();
	void LoadSaveFromFile(int32 SaveId);
	void OnPostLoadMapWithWorld(UWorld* LoadedWorld);
	void DeserializeActor(AActor* Actor, const FActorSaveData* ActorSaveData);
	FString GetSaveFilePath(int32 SaveId) const;
	int32 GetNextSaveId() const;
	void OnActorSpawned(AActor* SpawnedActor);
	void NotifyActorsAndComponents(AActor* Actor);


	FGameSaveData GameSaveData;
	FString SaveDirectoryName;
	TArray<int32> SaveIds;
	FDelegateHandle OnActorSpawnedDelegateHandle;

	bool bUseCompressedSaves = false;
	/** Used to avoid double @OnLevelDeserialized invocation */
	bool bIgnoreOnActorSpawnedCallback = false;
};