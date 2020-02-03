// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame_GM.h"
#include "TimerManager.h"

ACoopGame_GM::ACoopGame_GM()
{
	BaseSpawnBotCount = 1;
	MaxWaveCount = 3;
	SpawnBotInterval = 0.5f;
	SpawnBotWaveInterval = 5.0f;
}

void ACoopGame_GM::StartPlay()
{
	Super::StartPlay();
	PrepareForNextWave();
}

void ACoopGame_GM::SpawnABot()
{
	SpawnNewBot();
	BotsToSpawnCount--;
	if (BotsToSpawnCount <= 0)
	{
		EndWave();
	}
}

void ACoopGame_GM::StartWave()
{
	WaveCount++;
	BotsToSpawnCount = WaveCount * BaseSpawnBotCount; // 越往后,每波生成越多
	GetWorldTimerManager().SetTimer(TH_BotSpawner, this, &ACoopGame_GM::SpawnABot, SpawnBotInterval, true, 0.0f);
}

void ACoopGame_GM::EndWave()
{
	GetWorldTimerManager().ClearTimer(TH_BotSpawner);
	PrepareForNextWave();
}

// 在上一波结束时,开启定时器,生成下一波
void ACoopGame_GM::PrepareForNextWave()
{
	if (WaveCount >= MaxWaveCount)
	{
		return;
	}
	FTimerHandle TH_NextWaveStart;
	GetWorldTimerManager().SetTimer(TH_NextWaveStart, this, &ACoopGame_GM::StartWave, SpawnBotWaveInterval, false);
}
