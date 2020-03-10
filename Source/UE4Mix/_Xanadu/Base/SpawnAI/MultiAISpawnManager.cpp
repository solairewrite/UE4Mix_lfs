// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiAISpawnManager.h"
#include "_Xanadu/Characters/Enemy/Base/AICharacterBase.h"

void AMultiAISpawnManager::SpawnAI()
{

}

void AMultiAISpawnManager::ClearDeadAI()
{
	for (int i = CurrentAIArr.Num() - 1; i > 0; i--)
	{
		AAICharacterBase* tAI = CurrentAIArr[i];
		if (!tAI || tAI->GetHealth() <= 0.0f)
		{
			CurrentAIArr.RemoveAt(i);
		}
	}
	CurrentAICount = CurrentAIArr.Num();
}

void AMultiAISpawnManager::DestroyAllAI()
{
	for (AAICharacterBase* tAI : CurrentAIArr)
	{
		tAI->Destroy();
	}
	CurrentAIArr.Empty();
}
