// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Characters/Enemy/Base/AICharacterBase.h"
#include "AICharacter_Fox.generated.h"

class ACharacter;
class UNavigationPath;

/**
 *
 */
UCLASS()
class UE4MIX_API AAICharacter_Fox : public AAICharacterBase
{
	GENERATED_BODY()

public:
	AAICharacter_Fox();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	// AI寻路的目标位置
	// 防止由于路径点更新较慢,AI旋转卡顿
	FVector TargetLoc;
	// 寻路的目标位置差值到路径点的速度
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float TargetLocInterpSpeed;

	FVector NextPathPoint;
	FVector GetNextPathPoint();

	FTimerHandle TH_RefreshPath;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float RefreshPathInterval;
	void RefreshPath();

	void MoveToPlayer(float DeltaTime);

	void DebugDrawPath(UNavigationPath* inPath);

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float AccelerateSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float MeleeRange;

	ACharacter* Player;
};
