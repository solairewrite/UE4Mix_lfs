// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPoint.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "CableComponent.h"
#include "Materials/Material.h"

// Sets default values
APatrolPoint::APatrolPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Point_Visualizer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Point_Visualizer"));
	Point_Visualizer->SetupAttachment(RootComponent);
	Point_Visualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UStaticMesh* tMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Spawn_System/Mesh/SM_Sphere1.SM_Sphere1'"));
	Point_Visualizer->SetStaticMesh(tMesh);
	UMaterialInstanceConstant* tMeshMat = LoadObject<UMaterialInstanceConstant>(NULL, TEXT("MaterialInstanceConstant'/Game/Spawn_System/Materials/MI_Path_Point.MI_Path_Point'"));
	Point_Visualizer->SetMaterial(0, tMeshMat);
	Point_Visualizer->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	Point_Visualizer->SetHiddenInGame(false);

	Path_Visualizer = CreateDefaultSubobject<UCableComponent>(TEXT("Path_Visualizer"));
	Path_Visualizer->SetupAttachment(RootComponent);
	Path_Visualizer->bAttachEnd = true;
	UMaterial* tCableMat = LoadObject<UMaterial>(NULL, TEXT("Material'/Game/Spawn_System/Materials/M_Spline_Path.M_Spline_Path'"));
	Path_Visualizer->SetMaterial(0, tCableMat);
	Path_Visualizer->CableWidth = 15.0f;
}

// Called when the game starts or when spawned
void APatrolPoint::BeginPlay()
{
	Super::BeginPlay();

	if (Path_Visualizer && NextPoint)
	{
		Path_Visualizer->SetAttachEndTo(NextPoint, FName("Point_Visualizer"), FName("Center"));
	}
}

