// Fill out your copyright notice in the Description page of Project Settings.


#include "LightManagerBase.h"
#include "TimerManager.h"
#include "LightBase.h"

// Sets default values
ALightManagerBase::ALightManagerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TurnOnLightInterval = 0.3f;
}

// Called when the game starts or when spawned
void ALightManagerBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALightManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALightManagerBase::TurnOnLightsInTurn()
{
	LightIndexToBeTurnOn = 0;

	GetWorldTimerManager().SetTimer(TH_TurnOnLight, this, &ALightManagerBase::TimerTurnOnALight, TurnOnLightInterval, true, 0.0f);
}

void ALightManagerBase::TimerTurnOnALight()
{
	ALightBase* tLight;
	if (LightArr.Num() > LightIndexToBeTurnOn &&
		(tLight = LightArr[LightIndexToBeTurnOn]) != nullptr)
	{
		tLight->TurnOnLight();
		LightIndexToBeTurnOn++;
		if (LightIndexToBeTurnOn >= LightArr.Num())
		{
			LightIndexToBeTurnOn = 0;
			GetWorldTimerManager().ClearTimer(TH_TurnOnLight);
		}
	}
	else
	{
		LightIndexToBeTurnOn = 0;
		GetWorldTimerManager().ClearTimer(TH_TurnOnLight);
	}
}

