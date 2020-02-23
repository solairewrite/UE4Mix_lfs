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

//protected:
//	virtual void BeginPlay() override;
//
//public:
//	virtual void Tick(float DeltaTime) override;

protected:
	// AI寻路的目标位置
	// 防止由于路径点更新较慢,AI旋转卡顿
	FVector FoxTargetLoc;
	// 寻路的目标位置差值到路径点的速度
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float FoxTargetLocInterpSpeed;

	FVector FoxNextPathPoint;
	FVector FoxGetNextPathPoint();

	FTimerHandle FoxTH_RefreshPath;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float FoxRefreshPathInterval;
	void FoxRefreshPath();

	void FoxMoveToPlayer(float DeltaTime);

	void FoxDebugDrawPath(UNavigationPath* inPath);

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float FoxMaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float FoxAccelerateSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float FoxMeleeRange;

	ACharacter* FoxPlayer;
};
