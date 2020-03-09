// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Base/SpawnAI/AISpawnManagerBase.h"
#include "SingleAISpawnManager.generated.h"

/**
 *
 */
UCLASS()
class UE4MIX_API ASingleAISpawnManager : public AAISpawnManagerBase
{
	GENERATED_BODY()

public:
	ASingleAISpawnManager();

protected:
	UPROPERTY(EditAnywhere, Category = "SpawnAI")
		TSubclassOf<class AAICharacterBase> AIClass;

	UPROPERTY(EditInstanceOnly, Category = "SpawnAI")
		TArray<class APatrolPoint*> PatrolPointArr;

	class AAICharacterBase* AISpawned;

protected:
	virtual void CheckSpawnAI() override;

	virtual void SpawnAI() override;

	virtual void ClearDeadAI() override;
};
