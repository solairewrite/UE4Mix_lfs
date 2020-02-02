// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopGame_SPowerupActor.generated.h"

UCLASS()
class UE4MIX_API ACoopGame_SPowerupActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACoopGame_SPowerupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 时间单位,避免用Tick消耗性能
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		float PowerupInterval;

	// 持续时间(Tick 次数)
	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
		int32 TickNum;

	FTimerHandle TimerHandle_PowerupTick;

	// 已经Tick的次数
	int32 TicksProcessed;

	UFUNCTION()
		void OnTickPowerup();

	UPROPERTY(ReplicatedUsing = OnRep_PowerupActive)
		bool bIsPowerupActive;
	UFUNCTION()
		void OnRep_PowerupActive();

	// 蓝图实现函数
	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
		void OnPowerupStateChanged(bool bActive);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActivatePowerup();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps);
	// 蓝图实现,C++调用蓝图
	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
		void OnActivate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
		void OnPowerupTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
		void OnExpired(); // 到期,失效
};
