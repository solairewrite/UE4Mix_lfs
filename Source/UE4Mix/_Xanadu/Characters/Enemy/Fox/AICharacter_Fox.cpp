// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter_Fox.h"
#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController_Fox.h"

static int32 DebugLevel;
FAutoConsoleVariableRef CVARDebugLevel(
	TEXT("DebugLevel"),
	DebugLevel,
	TEXT("调试等级,控制是否显示调试球等,数值越大,能显示的越多"),
	ECVF_Cheat
);

AAICharacter_Fox::AAICharacter_Fox()
{
	MeleeRange = 250.0f;
	TargetLocInterpSpeed = 1.0f;
	AccelerateSpeed = 500.0f;
	MaxWalkSpeed = 300.0f;
	RefreshPathInterval = 0.3f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AIControllerClass = AAIController_Fox::StaticClass();
}

void AAICharacter_Fox::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	Player = UGameplayStatics::GetPlayerCharacter(this, 0);

	GetWorldTimerManager().SetTimer(TH_RefreshPath, this, &AAICharacter_Fox::RefreshPath, RefreshPathInterval, true);
}

void AAICharacter_Fox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Player && GetHorizontalDistanceTo(Player) > MeleeRange)
	{
		MoveToPlayer(DeltaTime);
	}
}

FVector AAICharacter_Fox::GetNextPathPoint()
{
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), Player);

	// 绘制路径点
	DebugDrawPath(NavPath);

	if (NavPath && NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}

	// 寻路失败
	return GetActorLocation();
}

void AAICharacter_Fox::RefreshPath()
{
	NextPathPoint = GetNextPathPoint();
}

void AAICharacter_Fox::MoveToPlayer(float DeltaTime)
{
	TargetLoc = FMath::VInterpTo(TargetLoc, NextPathPoint, DeltaTime, TargetLocInterpSpeed);
	FVector dir = TargetLoc - GetActorLocation();
	dir.Normalize();
	GetCharacterMovement()->AddInputVector(dir * AccelerateSpeed * DeltaTime);

	if (DebugLevel > 0)
	{
		DrawDebugSphere(GetWorld(), TargetLoc, 10.0f, 12, FColor::Red, false, 1, 0, 1);
	}
}

void AAICharacter_Fox::DebugDrawPath(UNavigationPath* inPath)
{
	if (DebugLevel <= 0)
	{
		return;
	}
	if (!inPath)
	{
		return;
	}
	for (int i = 0; i < inPath->PathPoints.Num(); ++i)
	{
		float tRadius = 15.0f;
		DrawDebugSphere(GetWorld(), inPath->PathPoints[i], tRadius, 12, FColor::Yellow, false, 1, 0, 1);
	}
}
