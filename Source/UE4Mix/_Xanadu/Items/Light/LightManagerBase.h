// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightManagerBase.generated.h"

/**
 * 光照管理器基类,控制灯光,火焰等的开关
 */
UCLASS()
class UE4MIX_API ALightManagerBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditInstanceOnly, Category = "Light")
		TArray<class ALightBase*> LightArr;

	FTimerHandle TH_TurnOnLight;
	// 打开两盏灯之间的时间间隔
	UPROPERTY(EditAnywhere, Category = "Light")
		float TurnOnLightInterval;

	int32 LightIndexToBeTurnOn;

public:
	// 依次打开光照数组里面的光照
	void TurnOnLightsInTurn();

	void TimerTurnOnALight();

};
