// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AICommand.h"
#include "Kismet/KismetMathLibrary.h"

static int32 DebugLevel;
FAutoConsoleVariableRef CVARDebugLevel(
	TEXT("DebugLevel"),
	DebugLevel,
	TEXT("调试等级,控制是否显示调试球等,数值越大,能显示的越多"),
	ECVF_Cheat
);

// Sets default values
AAICharacterBase::AAICharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxWalkSpeed = 300.0f;
	AccelerateSpeed = 500.0f;

	MeleeRange = 250.0f;

	TargetLocInterpSpeed = 2.0f;
	RefreshPathInterval = 0.3f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

// Called every frame
void AAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 移动到玩家
	if (bMovingToPlayer)
	{
		TickMoveToPlayer(DeltaTime);
	}

	// 转向玩家
	if (bTurningToPlayer)
	{
		TickTurnToPlayer(DeltaTime);
	}
}

ACharacter* AAICharacterBase::GetPlayer()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (Player)
	{
		return Player;
	}

	return nullptr;
}

FVector AAICharacterBase::GetNextPathPoint()
{
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), GetPlayer());

	if (DebugLevel > 0)
	{
		DebugDrawPath(NavPath);
	}

	if (NavPath && NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}

	// 寻路失败
	return GetActorLocation();
}

void AAICharacterBase::RefreshPath()
{
	NextPathPoint = GetNextPathPoint();
}

void AAICharacterBase::DebugDrawPath(UNavigationPath* inPath)
{
	if (!inPath)
	{
		return;
	}
	for (int i = 0; i < inPath->PathPoints.Num(); ++i)
	{
		DrawDebugSphere(GetWorld(), inPath->PathPoints[i], 15.0f, 12, FColor::Yellow, false, 1.0f, 0, 1.0f);
	}
}

void AAICharacterBase::TickMoveToPlayer(float DeltaTime)
{
	ACharacter* player = GetPlayer();
	if (!player)
	{
		bMovingToPlayer = false;

		// 移动到玩家失败
		CommandFail();
	}
	if (GetDistanceTo(player) > MeleeRange)
	{
		// 移动到玩家的算法
		TargetLoc = FMath::VInterpTo(TargetLoc, NextPathPoint, DeltaTime, TargetLocInterpSpeed);
		FVector dir = TargetLoc - GetActorLocation();
		dir.Normalize();
		GetCharacterMovement()->AddInputVector(dir * AccelerateSpeed * DeltaTime);

		if (DebugLevel > 0)
		{
			DrawDebugSphere(GetWorld(), TargetLoc, 10.0f, 12, FColor::Red, false, 1.0f, 0, 1.0f);
		}
	}
	else
	{
		bMovingToPlayer = false;
		// 移动到玩家成功
		CommandSuccess();
	}
}

void AAICharacterBase::TickTurnToPlayer(float DeltaTime)
{
	ACharacter* player = GetPlayer();
	if (!player)
	{
		bTurningToPlayer = false;
		CommandFail();
	}

	// 计算旋转
	FRotator tLookAtPlayerRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), player->GetActorLocation());

	if (GetDistanceTo(player) > MeleeRange)
	{
		// 移动到玩家的算法
		TargetLoc = FMath::VInterpTo(TargetLoc, NextPathPoint, DeltaTime, TargetLocInterpSpeed);
		FVector dir = TargetLoc - GetActorLocation();
		dir.Normalize();
		GetCharacterMovement()->AddInputVector(dir * AccelerateSpeed * DeltaTime);

		if (DebugLevel > 0)
		{
			DrawDebugSphere(GetWorld(), TargetLoc, 10.0f, 12, FColor::Red, false, 1.0f, 0, 1.0f);
		}
	}
	else
	{
		bTurningToPlayer = false;
		CommandSuccess();
	}
}

void AAICharacterBase::SetCurrentCommand(AAICommand* inCommand)
{
	CurrentCommand = inCommand;
}

void AAICharacterBase::CommandSuccess()
{
	if (CurrentCommand &&
		CurrentCommand->GetCommandState() == ECommandState::Doing)
	{
		CurrentCommand->CommandSuccess();
	}
}

void AAICharacterBase::CommandFail()
{
	if (CurrentCommand &&
		CurrentCommand->GetCommandState() == ECommandState::Doing)
	{
		CurrentCommand->CommandFail();
	}
}

void AAICharacterBase::MoveToPlayer()
{
	// Tick()中开始移动到玩家
	bMovingToPlayer = true;

	GetWorldTimerManager().ClearTimer(TH_RefreshPath);
	GetWorldTimerManager().SetTimer(TH_RefreshPath, this, &AAICharacterBase::RefreshPath, RefreshPathInterval, true);
}

void AAICharacterBase::TurnToPlayer()
{
	// Tick()中开始转向玩家
	bTurningToPlayer = true;
}

