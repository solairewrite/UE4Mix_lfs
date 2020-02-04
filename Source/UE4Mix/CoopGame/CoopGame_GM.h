// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoopGame_GM.generated.h"


// 引用外部枚举
enum class EWaveState :uint8;

// 声明代理
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

/**
 *
 */
UCLASS()
class UE4MIX_API ACoopGame_GM : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACoopGame_GM();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	FTimerHandle TH_BotSpawner;
	FTimerHandle TH_NextWaveStart;

	// 检查是否还有Bot存活
	void CheckWaveState();

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
		int32 BaseSpawnBotCount;
	int32 BotsToSpawnCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
		int32 MaxWaveCount;
	int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
		float SpawnBotInterval;
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
		float SpawnBotWaveInterval;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void SpawnNewBot();

	void SpawnABot();

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	void CheckAnyPlayerAlive();

	void GameOver();

	void SetWaveState(EWaveState NewState);

public:
	// 代理函数,蓝图实现代理
	UPROPERTY(BlueprintAssignable, Category = "GameMode")
		FOnActorKilled OnActorKilled;

	void RestartDeadPlayers();
};
