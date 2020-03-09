// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPoint.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

// Sets default values
APatrolPoint::APatrolPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Point_Visualizer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Point_Visualizer"));
	Point_Visualizer->SetupAttachment(RootComponent);
	UStaticMesh* tMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Spawn_System/Mesh/SM_Sphere1.SM_Sphere1'"));
	Point_Visualizer->SetStaticMesh(tMesh);
	UMaterialInstanceConstant* tMat = LoadObject<UMaterialInstanceConstant>(NULL, TEXT("MaterialInstanceConstant'/Game/Spawn_System/Materials/MI_Path_Point.MI_Path_Point'"));
	Point_Visualizer->SetMaterial(0, tMat);
	Point_Visualizer->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	Point_Visualizer->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void APatrolPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

