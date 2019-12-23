// Fill out your copyright notice in the Description page of Project Settings.


#include "LiftCube_CubeManager.h"
#include "LiftCube_Cube.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
ALiftCube_CubeManager::ALiftCube_CubeManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RowCount = 11;
	RiseCubeInterval = 0.2f;
	RiseHeightStep = 25.0f;
}

// Called when the game starts or when spawned
void ALiftCube_CubeManager::BeginPlay()
{
	Super::BeginPlay();

	RowCount = RowCount / 2 * 2 + 1; // 转为奇数
	SpawnCubes();

	GetWorldTimerManager().SetTimer(TH_Rise, this, &ALiftCube_CubeManager::TimerRiseCubes, RiseCubeInterval, true, 0.0f);
}

// Called every frame
void ALiftCube_CubeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALiftCube_CubeManager::SpawnCubes()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ALiftCube_Cube* cube = GetWorld()->SpawnActor<ALiftCube_Cube>
		(CubeClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	CubeSize = cube->GetSize();

	int halfRowCount = (RowCount - 1) / 2;
	float StartOffsetX = -halfRowCount * CubeSize;

	for (int row = 0; row < RowCount; ++row)
	{
		float offsetX = StartOffsetX + row * CubeSize;
		for (int col = 0; col < RowCount; ++col)
		{
			float offsetY = StartOffsetX + col * CubeSize;
			FVector Offset = FVector(offsetX, offsetY, 0);
			FVector CubeLoc = GetActorLocation() + Offset;
			if (row == 0 && col == 0)
			{
				cube->SetActorLocation(CubeLoc);
			}
			else
			{
				cube = GetWorld()->SpawnActor<ALiftCube_Cube>(CubeClass,
					CubeLoc, FRotator::ZeroRotator, SpawnParams);
			}
			CubeArr.Add(cube);
		}
	}
}

void ALiftCube_CubeManager::TimerRiseCubes()
{
	const int FarestCubeUnit = 4; // 最远升起的Cube
	for (int i = 0; i < CubeArr.Num(); ++i)
	{
		ALiftCube_Cube* cube = CubeArr[i];
		int DistanceToPlayerCubeUnit = cube->DistanceToPlayerCubeUnit();
		if (DistanceToPlayerCubeUnit < FarestCubeUnit)
		{
			float height = (FarestCubeUnit - DistanceToPlayerCubeUnit) * RiseHeightStep;
			cube->Rise(height);
		}
		else
		{
			cube->Down();
		}
	}
}

