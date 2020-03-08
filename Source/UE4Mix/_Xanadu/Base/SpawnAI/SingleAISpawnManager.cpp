// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleAISpawnManager.h"
#include "_Xanadu/Characters/Enemy/Fox/AICharacter_Fox.h"
#include "TimerManager.h"

ASingleAISpawnManager::ASingleAISpawnManager()
{
	AIClass = AAICharacter_Fox::StaticClass();
}

void ASingleAISpawnManager::CheckSpawnAI()
{
	if (!IsCanSpawnAI())
	{
		return;
	}

	AISpawned = SpawnAAI(AIClass, GetActorLocation());
	if (AISpawned)
	{
		bHasSpawnAI = true;
		GetWorldTimerManager().ClearTimer(TH_CheckSpawnAI);
	}
}
