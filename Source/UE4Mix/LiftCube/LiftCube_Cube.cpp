// Fill out your copyright notice in the Description page of Project Settings.


#include "LiftCube_Cube.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Sets default values
ALiftCube_Cube::ALiftCube_Cube()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CubeComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeComp->SetCollisionObjectType(ECC_WorldDynamic);
	SetRootComponent(CubeComp);

	Size = 100.0f;
	Scale3D = FVector(1.5f, 1.5f, 5.0f);
	MoveSpeed = 1000.0f;
}

// Called when the game starts or when spawned
void ALiftCube_Cube::BeginPlay()
{
	Super::BeginPlay();

	CubeComp->SetRelativeScale3D(Scale3D);
	CubeComp->SetMaterial(0, DefaultMat);
}

// Called every frame
void ALiftCube_Cube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		FVector CubeCompTrans = CubeComp->RelativeLocation;
		if (FMath::Abs(CubeCompTrans.Z - TargetHeight) > 3.0f)
		{
			float zDir = FMath::Sign(TargetHeight - CubeCompTrans.Z);
			CubeCompTrans += FVector(0, 0, zDir) * MoveSpeed * DeltaTime;
			CubeComp->SetRelativeLocation(CubeCompTrans);
		}
}

void ALiftCube_Cube::SetDefaultMat()
{
	if (CubeComp->GetMaterial(0) != DefaultMat)
	{
		CubeComp->SetMaterial(0, DefaultMat);
	}
}

void ALiftCube_Cube::SetRiseMat()
{
	if (CubeComp->GetMaterial(0) != RiseMat)
	{
		CubeComp->SetMaterial(0, RiseMat);
	}
}

void ALiftCube_Cube::Rise(float RiseHeight)
{
	TargetHeight = RiseHeight;

	SetRiseMat();
}

void ALiftCube_Cube::Down()
{
	TargetHeight = 0.0f;

	SetDefaultMat();
}

bool ALiftCube_Cube::IsRise()
{
	return FMath::Abs(CubeComp->RelativeLocation.Z) > 0.1f;
}

float ALiftCube_Cube::GetSize()
{
	return Size * Scale3D.X;
}

APawn* ALiftCube_Cube::GetPlayer()
{
	if (!Player)
	{
		Player = UGameplayStatics::GetPlayerPawn(this, 0);
	}
	return Player;
}

int ALiftCube_Cube::DistanceToPlayerCubeUnit()
{
	FVector PlayerLoc = GetPlayer()->GetActorLocation();
	FVector Offset = PlayerLoc - GetActorLocation();
	Offset.Z = 0.0f;
	float dis = Offset.Size();
	// 距离玩家几个方块
	int disCubeUnit = (int)((dis + Size / 2) / Size);
	return disCubeUnit;
}

void ALiftCube_Cube::Destroyed()
{
	Super::Destroyed();
}