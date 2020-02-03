// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame_SPowerupActor.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACoopGame_SPowerupActor::ACoopGame_SPowerupActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PowerupInterval = 0.0f;
	TickNum = 0;

	bIsPowerupActive = false;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ACoopGame_SPowerupActor::BeginPlay()
{
	Super::BeginPlay();

}

void ACoopGame_SPowerupActor::OnTickPowerup()
{
	// C++调用蓝图,蓝图实现
	OnPowerupTicked();
	if (TicksProcessed >= TickNum)
	{
		OnExpired();
		bIsPowerupActive = false;
		OnRep_PowerupActive();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
	TicksProcessed++;
}

void ACoopGame_SPowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChanged(bIsPowerupActive);
}

// Called every frame
void ACoopGame_SPowerupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoopGame_SPowerupActor::ActivatePowerup(AActor* ActiveFor)
{
	OnActivate(ActiveFor);
	bIsPowerupActive = true;
	OnRep_PowerupActive();
	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ACoopGame_SPowerupActor::OnTickPowerup, PowerupInterval, true, 0.0f);
	}
	else
	{
		OnTickPowerup();
	}
}

// 注意后面的const
void ACoopGame_SPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACoopGame_SPowerupActor, bIsPowerupActive);
}