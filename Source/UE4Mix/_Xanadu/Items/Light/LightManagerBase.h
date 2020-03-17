// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightManagerBase.generated.h"

class ALightBase;

// 一排灯
USTRUCT()
struct FLightRow
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Light")
		TArray<ALightBase*> LightArr;
};

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
	// 一排排灯的数组
	UPROPERTY(EditInstanceOnly, Category = "Light")
		TArray<FLightRow> LightRowArr;

	FTimerHandle TH_TurnOnLight;
	// 打开两盏灯之间的时间间隔
	UPROPERTY(EditAnywhere, Category = "Light")
		float TurnOnLightInterval;

	int32 LightIndexToBeTurnOn;

public:
	// 依次打开光照数组里面的光照
	UFUNCTION(BlueprintCallable, Category = "Light")
		void TurnOnLightsInTurn();

	UFUNCTION(BlueprintCallable, Category = "Light")
		void TimerTurnOnALight();

};
