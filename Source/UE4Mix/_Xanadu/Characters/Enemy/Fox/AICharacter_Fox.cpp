// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter_Fox.h"
#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController_Fox.h"

//static int32 DebugLevel;
//FAutoConsoleVariableRef CVARDebugLevel(
//	TEXT("DebugLevel"),
//	DebugLevel,
//	TEXT("调试等级,控制是否显示调试球等,数值越大,能显示的越多"),
//	ECVF_Cheat
//);

AAICharacter_Fox::AAICharacter_Fox()
{
	FoxMeleeRange = 250.0f;
	FoxTargetLocInterpSpeed = 1.0f;
	FoxAccelerateSpeed = 500.0f;
	FoxMaxWalkSpeed = 300.0f;
	FoxRefreshPathInterval = 0.3f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AIControllerClass = AAIController_Fox::StaticClass();
}

void AAICharacter_Fox::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = FoxMaxWalkSpeed;

	FoxPlayer = UGameplayStatics::GetPlayerCharacter(this, 0);

	GetWorldTimerManager().SetTimer(FoxTH_RefreshPath, this, &AAICharacter_Fox::FoxRefreshPath, FoxRefreshPathInterval, true);
}

void AAICharacter_Fox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FoxPlayer && GetHorizontalDistanceTo(FoxPlayer) > FoxMeleeRange)
	{
		FoxMoveToPlayer(DeltaTime);
	}
}

FVector AAICharacter_Fox::FoxGetNextPathPoint()
{
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), FoxPlayer);

	// 绘制路径点
	FoxDebugDrawPath(NavPath);

	if (NavPath && NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}

	// 寻路失败
	return GetActorLocation();
}

void AAICharacter_Fox::FoxRefreshPath()
{
	FoxNextPathPoint = FoxGetNextPathPoint();
}

void AAICharacter_Fox::FoxMoveToPlayer(float DeltaTime)
{
	FoxTargetLoc = FMath::VInterpTo(FoxTargetLoc, FoxNextPathPoint, DeltaTime, FoxTargetLocInterpSpeed);
	FVector dir = FoxTargetLoc - GetActorLocation();
	dir.Normalize();
	GetCharacterMovement()->AddInputVector(dir * FoxAccelerateSpeed * DeltaTime);

	//if (DebugLevel > 0)
	//{
		DrawDebugSphere(GetWorld(), FoxTargetLoc, 10.0f, 12, FColor::Red, false, 1, 0, 1);
	//}
}

void AAICharacter_Fox::FoxDebugDrawPath(UNavigationPath* inPath)
{
	//if (DebugLevel <= 0)
	//{
	//	return;
	//}
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
