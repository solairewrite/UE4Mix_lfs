// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame_GM.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "CoopGame_SHealthComponent.h"


ACoopGame_GM::ACoopGame_GM()
{
	BaseSpawnBotCount = 1;
	MaxWaveCount = 3;
	SpawnBotInterval = 0.5f;
	SpawnBotWaveInterval = 5.0f;

	PrimaryActorTick.bCanEverTick = true;
	// 设置Tick事件间隔
	PrimaryActorTick.TickInterval = 1.0f;
}

void ACoopGame_GM::StartPlay()
{
	Super::StartPlay();
	PrepareForNextWave();
}

void ACoopGame_GM::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
	CheckAnyPlayerAlive();
}

void ACoopGame_GM::CheckWaveState()
{
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TH_NextWaveStart);
	if (BotsToSpawnCount > 0 || bIsPreparingForWave)
	{
		return;
	}
	bool bIsAnyBotAlive = false;
	// 迭代Pawn,检测是否还有Bot
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TestPawn = It->Get();
		// 排除玩家,仅留下Bot
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled())
		{
			continue;
		}
		UCoopGame_SHealthComponent* HealthComp = Cast<UCoopGame_SHealthComponent>(TestPawn->GetComponentByClass(UCoopGame_SHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() > 0.0f)
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		PrepareForNextWave();
	}
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
	//PrepareForNextWave();
}

// 在上一波结束时,开启定时器,生成下一波
void ACoopGame_GM::PrepareForNextWave()
{
	if (WaveCount >= MaxWaveCount)
	{
		return;
	}

	GetWorldTimerManager().SetTimer(TH_NextWaveStart, this, &ACoopGame_GM::StartWave, SpawnBotWaveInterval, false);
}

void ACoopGame_GM::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			UCoopGame_SHealthComponent* HealthComp = Cast<UCoopGame_SHealthComponent>(MyPawn->GetComponentByClass(UCoopGame_SHealthComponent::StaticClass()));
			// ensure 作用类似于断点,确保HealthComp存在
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{
				// 有玩家存活
				return;
			}
		}
	}
	// 没有玩家存活
	GameOver();
}

void ACoopGame_GM::GameOver()
{
	EndWave();
	UE_LOG(LogTemp, Warning, TEXT("游戏失败! 玩家死亡"));
}
