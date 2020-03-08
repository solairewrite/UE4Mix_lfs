// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawnManagerBase.h"
#include "TimerManager.h"

// Sets default values
AAISpawnManagerBase::AAISpawnManagerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnAIRadius = 1500.0f;
	AIWanderRadius = 500.0f;
}

// Called when the game starts or when spawned
void AAISpawnManagerBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TH_CheckSpawnAI, this, &AAISpawnManagerBase::CheckSpawnAI, 0.2f, true);
}

// Called every frame
void AAISpawnManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAISpawnManagerBase::CheckSpawnAI()
{

}

bool AAISpawnManagerBase::IsCanSpawnAI()
{
	if (bHasSpawnAI)
	{
		return false;
	}
	return true;
}

AAICharacterBase* AAISpawnManagerBase::SpawnAAI(TSubclassOf<AAICharacterBase> inAIClass, FVector inSpawnLoc)
{
	return nullptr;
}

