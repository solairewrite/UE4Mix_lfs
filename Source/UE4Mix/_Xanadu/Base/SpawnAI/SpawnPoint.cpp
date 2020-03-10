// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Materials/Material.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	CreatePointVisualizer();
	CreateArrow();
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

}

void ASpawnPoint::CreatePointVisualizer()
{
	Point_Visualizer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Point_Visualizer"));
	Point_Visualizer->SetupAttachment(RootComponent);
	Point_Visualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UStaticMesh* tMesh = LoadObject<UStaticMesh>(NULL,
		TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	Point_Visualizer->SetStaticMesh(tMesh);

	UMaterial* tMeshMat = LoadObject<UMaterial>(NULL,
		TEXT("Material'/Game/Spawn_System/Materials/M_stationary_Point.M_stationary_Point'"));
	Point_Visualizer->SetMaterial(0, tMeshMat);

	Point_Visualizer->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	Point_Visualizer->SetHiddenInGame(true);
}

void ASpawnPoint::CreateArrow()
{
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(Point_Visualizer);
	ArrowComp->SetRelativeLocation(FVector(40.0f, 0, 0));
}

