// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Base/SpawnAI/AISpawnManagerBase.h"
#include "MultiAISpawnManager.generated.h"

/**
 *
 */
UCLASS()
class UE4MIX_API AMultiAISpawnManager : public AAISpawnManagerBase
{
	GENERATED_BODY()

protected:
	TArray<class AAICharacterBase*> CurrentAIArr;

	TArray<class ASpawnPoint*> SpawnPointArr;

protected:
	virtual void SpawnAI() override;

	virtual void ClearDeadAI() override;

	virtual void DestroyAllAI() override;
};
