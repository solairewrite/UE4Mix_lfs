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
	// 编辑器中显示的Icon
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UBillboardComponent* IconBillboardComp;

	// 显示生成范围的球体,并不进行碰撞检测
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USphereComponent* ShowSpawnRadiusSphereComp;

	UPROPERTY(EditAnywhere, Category = "SpawnAI")
		int32 MaxSpawnAICount;

	UPROPERTY(EditAnywhere, Category = "SpawnAI")
		bool bCanRepeatSpawnAI;

	UPROPERTY(EditAnywhere, Category = "SpawnAI")
		float SpawnAICD;

	int32 CurrentAICount;
	int32 AllSpawnedAICount;

	float LastSpawnAITime;

	//TArray<class AAICharacterBase*> CurrentAIArr;

	// 玩家进入多少范围内,SpawnAI
	UPROPERTY(EditAnywhere, Category = "SpawnAI")
		float SpawnAIRadius;

	// 玩家离开多少范围内,销毁所有AI
	UPROPERTY(EditAnywhere, Category = "SpawnAI")
		float KillAIRadius;

	UPROPERTY(EditAnywhere, Category = "SpawnAI")
		float AIWanderRadius;

protected:
	FTimerHandle TH_CheckSpawnAI;
	// 定时检测是否可以生成AI,如果可以则生成
	virtual void CheckSpawnAI();

	virtual bool IsCanSpawnAI();

	// 生成AI,可能生成一个或多个,子类需要重写此函数
	virtual void SpawnAI();

	virtual AAICharacterBase* SpawnAAI(TSubclassOf<AAICharacterBase> inAIClass, FVector inSpawnLoc, FRotator inSpawnRot = FRotator::ZeroRotator);

	void DestroyAllAI();

	float GetDistanceToPlayer();

	// 清除死亡AI的引用和更新数量
	virtual void ClearDeadAI();
};
