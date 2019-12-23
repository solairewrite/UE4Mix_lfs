// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LiftCube_CubeManager.generated.h"

class ALiftCube_Cube;

UCLASS()
class UE4MIX_API ALiftCube_CubeManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALiftCube_CubeManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Cube")
		TSubclassOf<ALiftCube_Cube> CubeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Cube")
		int RowCount; // 行数,列数,必须为奇数

	TArray<ALiftCube_Cube*> CubeArr;

	float CubeSize;

	void SpawnCubes();

	UPROPERTY(EditDefaultsOnly, Category = "Cube")
		float RiseCubeInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Cube")
		float RiseHeightStep;

	FTimerHandle TH_Rise;
	void TimerRiseCubes();
};
