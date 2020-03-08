// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawnManagerBase.generated.h"

class AAICharacterBase;

UCLASS()
class UE4MIX_API AAISpawnManagerBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAISpawnManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 玩家进入多少范围内,SpawnAI
	UPROPERTY(EditAnywhere, Category = "SpawnAI")
		float SpawnAIRadius;

	UPROPERTY(EditAnywhere, Category = "SpawnAI")
		float AIWanderRadius;

	bool bHasSpawnAI;

protected:
	FTimerHandle TH_CheckSpawnAI;
	// 定时检测是否可以生成AI,如果可以则生成
	virtual void CheckSpawnAI();

	virtual bool IsCanSpawnAI();

	virtual AAICharacterBase* SpawnAAI(TSubclassOf<AAICharacterBase> inAIClass, FVector inSpawnLoc);
};
