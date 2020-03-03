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
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "_Xanadu/Characters/Base/Components/HealthComponent.h"
#include "UE4Mix.h"

// 声明控制台变量有两种方式:
// 1,直接注册控制台变量
// 2,注册一个对现有变量的引用

// 注册引用
//static int32 DebugLevel;
//FAutoConsoleVariableRef CVARDebugLevel(
//	TEXT("DebugLevel"),
//	DebugLevel,
//	TEXT("调试等级,控制是否显示调试球等,数值越大,能显示的越多"),
//	ECVF_Cheat
//);

// 引用已经创建的控制台变量,需要#include".h"
extern TAutoConsoleVariable<int32> CVARDebugLevel;

// Sets default values
AAICharacterBase::AAICharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 运动
	MaxWalkSpeed = 300.0f;
	AccelerateSpeed = 500.0f;
	RotateSpeed = 120.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 寻路
	TargetLocInterpSpeed = 2.0f;
	RefreshPathInterval = 0.3f;

	// 攻击
	MeleeRange = 250.0f;

	// 血量组件
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	if (HealthComp)
	{
		// 绑定代理函数,当调用OnTakeAnyDamage.Broadcast时,会触发代理函数
		OnTakeAnyDamage.AddDynamic(HealthComp, &UHealthComponent::OnTakeDamage);
	}
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

	if (bDead)
	{
		return;
	}

	// 判断命令状态,防止命令被暂停
	if (CurrentCommand->GetCommandState() == ECommandState::Doing)
	{
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
}

void AAICharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AAIControllerBase* controller = GetController<AAIControllerBase>();
	if (controller)
	{
		controller->GC_CommandManager();
		controller->GC_AnimManager();
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

	if (CVARDebugLevel.GetValueOnGameThread() > 0)
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

void AAICharacterBase::DebugDrawRotateInfo(ACharacter* inPlayer)
{
	FVector lineStart = GetActorLocation();
	FVector forwardLineEnd = lineStart + GetActorForwardVector() * 250.0f;

	FVector tDir = inPlayer->GetActorLocation() - GetActorLocation();
	tDir.Normalize();
	FVector targetLineEnd = lineStart + tDir * 250.0f;

	DrawDebugLine(GetWorld(), lineStart, forwardLineEnd, FColor::Orange, false, 1.0f, 0, 1.0f);
	DrawDebugLine(GetWorld(), lineStart, targetLineEnd, FColor::Green, false, 1.0f, 0, 1.0f);
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

		if (CVARDebugLevel.GetValueOnGameThread() > 0)
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
		return;
	}

	// 计算旋转,下面的计算基于,假设Yaw的取值范围是[0,360]
	FRotator tLookAtPlayerRot = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(), player->GetActorLocation());
	tLookAtPlayerRot.Roll = 0;
	tLookAtPlayerRot.Pitch = 0;

	// 计算旋转方向,向右旋转(顺时针)增加Yaw值
	// 向左旋转(逆时针)减少Yaw值
	int addRotDirection = CalcRotateDirection(tLookAtPlayerRot);
	// 计算锐角夹角[0,180)
	float YawDelta = FMath::Abs(GetActorRotation().Yaw - tLookAtPlayerRot.Yaw);
	if (YawDelta >= 180.0f)
	{
		YawDelta -= 180.0f;
	}

	// 判断是否已经朝向玩家
	float YawThreshold = 15.0f;
	if (YawDelta > YawThreshold)
	{
		float tYaw = RotateSpeed * addRotDirection * DeltaTime;
		tYaw += GetActorRotation().Yaw;
		SetActorRotation(FRotator(0, tYaw, 0));

		if (CVARDebugLevel.GetValueOnGameThread() > 0)
		{
			DebugDrawRotateInfo(player);
		}
	}
	else
	{
		bTurningToPlayer = false;
		CommandSuccess();
	}
}

// 向右旋转(顺时针)增加Yaw值,返回+1
// 向左旋转(逆时针)减少Yaw值,返回-1
int AAICharacterBase::CalcRotateDirection(FRotator inRotator)
{
	int addRotDirection = 0;
	float YawDelta = FMath::Abs(GetActorRotation().Yaw - inRotator.Yaw);
	if (YawDelta < 180.0f)
	{
		addRotDirection = (inRotator.Yaw > GetActorRotation().Yaw) ? 1 : -1;
	}
	else
	{
		addRotDirection = (inRotator.Yaw > GetActorRotation().Yaw) ? -1 : 1;
	}

	RotateDirection = ERotateDirection::None;
	if (addRotDirection == 1)
	{
		RotateDirection = ERotateDirection::Right;
	}
	else if (addRotDirection == -1)
	{
		RotateDirection = ERotateDirection::Left;
	}

	return addRotDirection;
}

void AAICharacterBase::OnAnimMontageEnd(UAnimMontage* inMontage, bool bInterrupted)
{
	AAIControllerBase* controller = GetController<AAIControllerBase>();
	if (!controller)
	{
		return;
	}

	if (!MontageNameMap.Contains(inMontage))
	{
		return;
	}

	FName animName = MontageNameMap[inMontage];
	if (animName.IsNone())
	{
		return;
	}

	if (animName == FName("Death"))
	{
		controller->GC_CommandManager();
		controller->GC_AnimManager();

		return;
	}

	if (!bInterrupted)
	{
		controller->OnPlayAnimSuccess(animName);
	}
}

void AAICharacterBase::SetCurrentCommand(AAICommand* inCommand)
{
	CurrentCommand = inCommand;
}

void AAICharacterBase::CommandSuccess()
{
	MontageNameMap.Empty();

	if (CurrentCommand &&
		CurrentCommand->GetCommandState() == ECommandState::Doing)
	{
		CurrentCommand->CommandSuccess();
	}
}

void AAICharacterBase::CommandFail()
{
	MontageNameMap.Empty();

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

float AAICharacterBase::PlayAnim(FName inAnimName, bool bPlayImmediately /*= false*/)
{
	if (!AnimMap.Contains(inAnimName))
	{
		return 0;
	}
	UAnimSequenceBase* seq = AnimMap[inAnimName];
	if (!seq)
	{
		return 0;
	}

	USkeletalMeshComponent* mesh = FindComponentByClass<USkeletalMeshComponent>();
	if (!mesh)
	{
		return 0;
	}

	UAnimInstance* animInst = mesh->GetAnimInstance();
	if (!animInst)
	{
		return 0;
	}

	//if (bPlayImmediately)
	//{
	//	StopAnimMontage();
	//}
	UAnimMontage* montage = animInst->PlaySlotAnimationAsDynamicMontage(seq, TEXT("DefaultSlot"));
	if (!montage)
	{
		return 0;
	}
	MontageNameMap.Add(montage, inAnimName);

	// 添加代理,动画结束回调
	animInst->OnMontageEnded.Clear();
	animInst->OnMontageEnded.AddDynamic(this, &AAICharacterBase::OnAnimMontageEnd);

	float animLength = montage->GetPlayLength();
	return animLength;
}

float AAICharacterBase::GetHealth()
{
	if (HealthComp)
	{
		return HealthComp->GetHealth();
	}

	return 0.0f;
}

//float AAICharacterBase::GetHealth_Implementation()
//{
//	if (HealthComp)
//	{
//		return HealthComp->GetHealth();
//	}
//
//	return 0.0f;
//}

bool AAICharacterBase::IsAI()
{
	return true;
}

void AAICharacterBase::OnDead()
{
	bDead = true;
	float tAnimLength = PlayAnim("Death");
	SetLifeSpan(tAnimLength + 1.0f);
}

void AAICharacterBase::OnDeathAnimEnd()
{
	// 如果是用Slot播放死亡Montage,则死亡动画结束后会回到Idle动画
	// 暂停动画,需要在死亡AnimSequence后面加上10个空帧(30帧/s,动画淡出时间0.25s)
	if (USkeletalMeshComponent* tMesh = GetMesh())
	{
		tMesh->bPauseAnims = true;
	}
}

void AAICharacterBase::OnMelee()
{

}

bool AAICharacterBase::CanPlayTakeHitAnim()
{
	return false;
}

void AAICharacterBase::PlayTakeHitAnim()
{
	AAIControllerBase* controller = GetController<AAIControllerBase>();
	if (controller)
	{
		controller->PlayAnimImmediately("TakeHit");
	}
}

bool AAICharacterBase::CanTakeImpulse()
{
	return false;
}

void AAICharacterBase::TakeImpulse(FVector inImpulseVector)
{
	LaunchCharacter(inImpulseVector, false, false);
}

