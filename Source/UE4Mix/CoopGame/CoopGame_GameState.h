// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CoopGame_GameState.generated.h"


UENUM(BlueprintType)
enum class EWaveState :uint8
{
	WaitingToStart,
	WaveInProgress,
	WaitingToComplete, // 不再生成Bot,等待玩家杀死机器人
	GameOver,
};

/**
 *
 */
UCLASS()
class UE4MIX_API ACoopGame_GameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UFUNCTION()
		void OnRep_WaveState(EWaveState OldState);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState")
		void WaveStateChanged(EWaveState NewState, EWaveState OldState);

public:
	// 同步函数可以传递参数旧值
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
		EWaveState WaveState;

	void SetState(EWaveState NewState);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
