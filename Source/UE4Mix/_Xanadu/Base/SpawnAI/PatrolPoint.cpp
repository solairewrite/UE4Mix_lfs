// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPoint.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "CableComponent.h"
#include "Materials/Material.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

APatrolPoint::APatrolPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	CreatePointVisualizer();
	CreatePathVisualizer();
}

#if WITH_EDITOR
void APatrolPoint::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 获取修改的变量名
	FName tName = (PropertyChangedEvent.Property != nullptr) ?
		PropertyChangedEvent.Property->GetFName() :
		NAME_None;
	// 检测变量名,获取变量名
	if (tName == GET_MEMBER_NAME_CHECKED(APatrolPoint, NextPoint))
	{
		SetPath();
	}
}
#endif

void APatrolPoint::BeginPlay()
{
	Super::BeginPlay();
}

void APatrolPoint::CreatePointVisualizer()
{
	Point_Visualizer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Point_Visualizer"));
	Point_Visualizer->SetupAttachment(RootComponent);
	Point_Visualizer->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UStaticMesh* tMesh = LoadObject<UStaticMesh>(NULL, 
		TEXT("StaticMesh'/Game/Spawn_System/Mesh/SM_Sphere1.SM_Sphere1'"));
	Point_Visualizer->SetStaticMesh(tMesh);

	UMaterialInstanceConstant* tMeshMat = LoadObject<UMaterialInstanceConstant>(NULL, 
		TEXT("MaterialInstanceConstant'/Game/Spawn_System/Materials/MI_Path_Point.MI_Path_Point'"));
	Point_Visualizer->SetMaterial(0, tMeshMat);

	Point_Visualizer->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	Point_Visualizer->SetHiddenInGame(true);
}

void APatrolPoint::CreatePathVisualizer()
{
	Path_Visualizer = CreateDefaultSubobject<UCableComponent>(TEXT("Path_Visualizer"));
	Path_Visualizer->SetupAttachment(RootComponent);

	UMaterial* tCableMat = LoadObject<UMaterial>(NULL, 
		TEXT("Material'/Game/Spawn_System/Materials/M_Spline_Path.M_Spline_Path'"));
	Path_Visualizer->SetMaterial(0, tCableMat);

	Path_Visualizer->CableWidth = 50.0f; // 实测开始修改宽度会导致后面宽度不显示
	Path_Visualizer->CableLength = 0.0f;
	Path_Visualizer->SolverIterations = 16; // 不弯曲的程度
	Path_Visualizer->SetRelativeLocation(FVector::ZeroVector);
	Path_Visualizer->bAttachEnd = false;
	Path_Visualizer->EndLocation = FVector::ZeroVector;
	Path_Visualizer->SetHiddenInGame(true);

	SetPath();
}

void APatrolPoint::SetPath()
{
	if (!Path_Visualizer)
	{
		return;
	}
	Path_Visualizer->SetRelativeLocation(FVector::ZeroVector);
	Path_Visualizer->SetRelativeScale3D(FVector::OneVector);
	if (NextPoint)
	{
		Path_Visualizer->bAttachEnd = true;
		Path_Visualizer->SetAttachEndTo(NextPoint, FName("Point_Visualizer"), FName("Center"));
		Path_Visualizer->CableLength = 100.0f;
	}
	else
	{
		Path_Visualizer->bAttachEnd = false;
		Path_Visualizer->SetAttachEndTo(nullptr, NAME_None);
		Path_Visualizer->EndLocation = FVector::ZeroVector;
		Path_Visualizer->CableLength = 0.0f;
	}
}

