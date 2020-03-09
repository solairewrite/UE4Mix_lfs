// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleAISpawnManager.h"
#include "_Xanadu/Characters/Enemy/Fox/AICharacter_Fox.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "_Xanadu/Characters/Base/Interfaces/IHealth.h"

ASingleAISpawnManager::ASingleAISpawnManager()
{
	AIClass = AAICharacter_Fox::StaticClass();
}

void ASingleAISpawnManager::CheckSpawnAI()
{
	Super::CheckSpawnAI();
}

void ASingleAISpawnManager::SpawnAI()
{
	AISpawned = SpawnAAI(AIClass, GetActorLocation(), GetActorRotation());
	LastSpawnAITime = GetWorld()->TimeSeconds;

	if (AISpawned->CanPatrol())
	{
		AISpawned->SetPatrolPointArr(PatrolPointArr);
	}
}

void ASingleAISpawnManager::ClearDeadAI()
{
	if (!AISpawned || IIHealth::Execute_GetHealth(AISpawned) <= 0)
	{
		AISpawned = nullptr;
		CurrentAICount = 0;
	}
}
