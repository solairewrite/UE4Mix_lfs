// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacterBase.generated.h"

class AIControllerBase;
class ACharacter;
class UNavigationPath;
class AAICommand;

UCLASS()
class UE4MIX_API AAICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AICharacter")
		float MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "AICharacter")
		float AccelerateSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "AICharacter")
		float MeleeRange;

	// TODO 新建Player基类
	ACharacter* GetPlayer();
	template<class T>
	T* GetPlayer() const
	{
		return Cast<T>(GetPlayer());
	}

	AAICommand* CurrentCommand;

	// Tick()中决定是否移动到玩家
	bool bMovingToPlayer;
	// Tick()中决定是否转向玩家
	bool bTurningToPlayer;

	// AI移动的目标位置,是两次寻路位置的差值
	// 防止突然改变目标位置造成的训传卡顿
	FVector TargetLoc;
	UPROPERTY(EditDefaultsOnly, Category = "AICharacter")
		float TargetLocInterpSpeed;

	// AI寻路的路径点,实际移动的目标位置TargetLoc,是两个路径点的差值
	FVector NextPathPoint;
	FVector GetNextPathPoint();

	FTimerHandle TH_RefreshPath;
	UPROPERTY(EditDefaultsOnly, Category = "AICharacter")
		float RefreshPathInterval;
	void RefreshPath();

	void DebugDrawPath(UNavigationPath* inPath);

	void TickMoveToPlayer(float DeltaTime);

	void TickTurnToPlayer(float DeltaTime);

public:
	void SetCurrentCommand(AAICommand* inCommand);

	// 当前任务成功
	void CommandSuccess();

	// 当前任务失败
	void CommandFail();

	void MoveToPlayer();

	void TurnToPlayer();

};
