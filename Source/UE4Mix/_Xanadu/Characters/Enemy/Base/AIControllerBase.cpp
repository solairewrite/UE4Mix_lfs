// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "CommandManager.h"
#include "Engine/World.h"
#include "AIAnimManager.h"
#include "AICommand.h"
#include "_Xanadu/Base/XanaduTypes.h"
#include "Kismet/KismetMathLibrary.h"
#include "AICharacterBase.h"
#include "TimerManager.h"
#include "_Xanadu/Base/XanaduTools.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "_Xanadu/Characters/Player/Base/PlayerCharacterBase.h"
#include "_Xanadu/Characters/Player/Base/PlayerControllerBase.h"

extern TAutoConsoleVariable<int32> CVARDebugLevel;

AAIControllerBase::AAIControllerBase()
{
	AIState = EAIState::Idle;

	RememberDamageTime = 10.0f;
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	InitAnimManager();

	InitCommandManager();
	// 根据感知系统开启命令管理器
	//StartCommand();

	InitAIPerception();

	StartIdle();
}

void AAIControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CVARDebugLevel.GetValueOnGameThread() > 0)
	{
		DrawDebugInfo();
	}
}

void AAIControllerBase::InitAnimManager()
{
	FActorSpawnParameters SpawnPara;
	SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AnimManager = GetWorld()->SpawnActor<AAIAnimManager>
		(AAIAnimManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnPara);

	if (AnimManager)
	{
		AnimManager->Init(this);
	}
}

void AAIControllerBase::InitCommandManager()
{
	if (CommandManagerClass == nullptr)
	{
		return;
	}

	FActorSpawnParameters SpawnPara;
	SpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CommandManager = GetWorld()->SpawnActor<ACommandManager>
		(CommandManagerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnPara);

	if (CommandManager)
	{
		CommandManager->Init(this);
	}
}

void AAIControllerBase::StartCommand()
{
	if (CommandManager)
	{
		CommandManager->StartAction();
	}
}

void AAIControllerBase::PrepareForPlayAnim(AAICommand* inCommand)
{
	if (AnimManager)
	{
		AnimManager->InitCommand(inCommand);
	}
}

void AAIControllerBase::PlayAnim(FName inAnimName)
{
	if (AnimManager)
	{
		AnimManager->PlayAnim(inAnimName);
	}
}

void AAIControllerBase::PlayAnimImmediately(FName inAnimName)
{
	if (AnimManager)
	{
		AnimManager->PlayAnimImmediately(inAnimName);
	}
}

void AAIControllerBase::OnPlayAnimSuccess(FName inAnimName)
{
	if (AnimManager)
	{
		AnimManager->OnPlayAnimSuccess(inAnimName);
	}
}

void AAIControllerBase::OnPlayAnimFail(FName inAnimName)
{

}

void AAIControllerBase::GC_CommandManager()
{
	if (CommandManager)
	{
		CommandManager->Destroy();
	}
}

void AAIControllerBase::GC_AnimManager()
{
	if (AnimManager)
	{
		AnimManager->Destroy();
	}
}

void AAIControllerBase::OnAIStateChanged(EAIState OldState, EAIState NewState)
{
	if (CVARDebugLevel.GetValueOnGameThread() > 0)
	{
		FString tInfo = FString::Printf(TEXT("%s -> %s"),
			*XanaduTools::EAIStateToString(OldState),
			*XanaduTools::EAIStateToString(NewState));
		GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Blue, tInfo);
	}

	if (OldState != NewState)
	{
		OnLeaveAIState(OldState);
	}
	OnEnterAIState(NewState);
}

void AAIControllerBase::OnEnterAIState(EAIState inState)
{
	switch (inState)
	{
	case EAIState::None:
		break;
	case EAIState::Escape:
		break;
	case EAIState::Sleep:
		break;
	case EAIState::Rest:
		break;
	case EAIState::Idle:
		OnEnterIdleState();
		break;
	case EAIState::Patrol:
		OnEnterPatrolState();
		break;
	case EAIState::Alert:
		break;
	case EAIState::SearchTarget:
		break;
	case EAIState::Attack:
		OnEnterAttackState();
		break;
	default:
		break;
	}
}

void AAIControllerBase::OnLeaveAIState(EAIState inState)
{
	switch (inState)
	{
	case EAIState::None:
		break;
	case EAIState::Escape:
		break;
	case EAIState::Sleep:
		break;
	case EAIState::Rest:
		break;
	case EAIState::Idle:
		OnLeaveIdleState();
		break;
	case EAIState::Patrol:
		break;
	case EAIState::Alert:
		break;
	case EAIState::SearchTarget:
		break;
	case EAIState::Attack:
		OnLeaveAttackState();
		break;
	default:
		break;
	}
}

void AAIControllerBase::StartIdle()
{
	SetAIState(EAIState::Idle);
}

void AAIControllerBase::OnEnterIdleState()
{
	float tIdleTime = 0.1f;
	AAICharacterBase* character = GetCharacter<AAICharacterBase>();
	// 动态Spawn出来的AI,在第一帧获取不到character,不能直接返回,否则会导致后面的寻路失败
	if (character)
	{
		float tMin = character->GetIdleTimeRangeMin();
		float tMax = character->GetIdleTimeRangeMax();
		tMin = FMath::Max(0.1f, tMin);
		tMax = FMath::Max(tMin, tMax);
		tIdleTime = UKismetMathLibrary::RandomFloatInRange(tMin, tMax);
	}
	// 定时结束Idle状态
	GetWorldTimerManager().SetTimer(TH_FinishIdle, this, &AAIControllerBase::FinishIdle, tIdleTime, false);
}

void AAIControllerBase::OnLeaveIdleState()
{
	GetWorldTimerManager().ClearTimer(TH_FinishIdle);
}

void AAIControllerBase::FinishIdle()
{
	EAIState tNextState = DecideNextNormalState();
	SetAIState(tNextState);
}

void AAIControllerBase::OnEnterPatrolState()
{
	AAICharacterBase* character = GetCharacter<AAICharacterBase>();
	if (character)
	{
		character->StartPartol();
	}
}

void AAIControllerBase::OnEnterAttackState()
{
	// 停止原来的移动,如巡逻
	StopMovement();
	StartCommand();
}

void AAIControllerBase::OnLeaveAttackState()
{
	if (CommandManager)
	{
		CommandManager->StopAction();
	}
}

EAIState AAIControllerBase::DecideNextNormalState()
{
	// 暂时只考虑Idle和巡逻,不考虑睡眠和休息状态
	AAICharacterBase* character = GetCharacter<AAICharacterBase>();
	if (!character)
	{
		return EAIState::None;
	}

	if (character->CanPatrol())
	{
		bool bCanPatrol = FMath::FRand() < character->GetPatrolProbability();
		if (bCanPatrol)
		{
			return EAIState::Patrol;
		}
	}

	return EAIState::Idle;
}

void AAIControllerBase::SetAIState(EAIState inNewState)
{
	if (AIState == inNewState)
	{
		// 即使状态相同也不返回,比如巡逻后还想继续巡逻
		//return;
	}

	EAIState tOldState = AIState;
	AIState = inNewState;

	OnAIStateChanged(tOldState, inNewState);
}

bool AAIControllerBase::IsNormalState()
{
	if (AIState == EAIState::Idle ||
		AIState == EAIState::Patrol ||
		AIState == EAIState::Rest ||
		AIState == EAIState::Sleep)
	{
		return true;
	}
	return false;
}

void AAIControllerBase::FinishPatrol()
{
	EAIState tNextState = DecideNextNormalState();
	SetAIState(tNextState);
}

UAIPerceptionComponent* AAIControllerBase::GetAIPerceptionComp()
{
	return Cast<UAIPerceptionComponent>(GetComponentByClass(UAIPerceptionComponent::StaticClass()));
}

void AAIControllerBase::InitAIPerception()
{
	UAIPerceptionComponent* AIPerceptionComp = GetAIPerceptionComp();
	if (!AIPerceptionComp)
	{
		return;
	}

	// 定时AI感知
	GetWorldTimerManager().SetTimer(TH_AIPerception, this, &AAIControllerBase::TimerAIPerception, 0.2f, true, 0.0f);
}

void AAIControllerBase::TimerAIPerception()
{
	UAIPerceptionComponent* AIPerceptionComp = GetAIPerceptionComp();
	if (!AIPerceptionComp)
	{
		return;
	}

	// 获取所有感知Actor,包括当前没看到,但是在MaxAge时间内看到的
	TArray<AActor*> tSensedActors;
	// 这里的感知类型不能填基类,要具体到视觉
	AIPerceptionComp->GetKnownPerceivedActors(UAISenseConfig_Sight::StaticClass(), tSensedActors);
	// 伤害感知实测不实用

	// 普通状态检测是否有攻击目标
	if (IsNormalState())
	{
		for (AActor* tActor : tSensedActors)
		{
			APlayerCharacterBase* tCharacter = Cast<APlayerCharacterBase>(tActor);
			if (tCharacter && IIHealth::Execute_GetHealth(tCharacter) > 0.0f)
			{
				FindAttackTarget(tCharacter);
				return;
			}
		}
	}

	// 战斗状态检测是否丢失攻击目标
	if (AIState == EAIState::Attack && !bRememberDamage)
	{
		if (AttackTarget == nullptr ||
			!tSensedActors.Contains(AttackTarget))
		{
			LoseAttackTarget();
			return;
		}
	}

	if (AIState == EAIState::Attack && AttackTarget &&
		IIHealth::Execute_GetHealth(AttackTarget) <= 0.0f)
	{
		LoseAttackTarget();
	}
}

void AAIControllerBase::FindAttackTarget(APlayerCharacterBase* inPlayer)
{
	AttackTarget = inPlayer;
	// 设置攻击状态,并在进入攻击状态的回调函数中,开启命令管理器
	SetAIState(EAIState::Attack);
}

void AAIControllerBase::LoseAttackTarget()
{
	AttackTarget = nullptr;
	// 设置进入Idle状态,并在离开攻击状态的回调函数中,关闭命令管理器
	SetAIState(EAIState::Idle);
}

void AAIControllerBase::DrawDebugInfo()
{

}

void AAIControllerBase::ForgetDamage()
{
	bRememberDamage = false;
}

void AAIControllerBase::OnAttackBy(class AController* InstigatedBy, AActor* DamageCauser)
{
	// 检测玩家直接造成了伤害
	APlayerCharacterBase* player = Cast<APlayerCharacterBase>(DamageCauser);

	if (!player)
	{
		// 检测玩家间接造成了伤害
		APlayerControllerBase* pc = Cast<APlayerControllerBase>(InstigatedBy);
		if (pc)
		{
			player = pc->GetCharacter<APlayerCharacterBase>();
		}
	}

	if (player)
	{
		bRememberDamage = true;
		GetWorldTimerManager().SetTimer(TH_RememberDamage, this, &AAIControllerBase::ForgetDamage, RememberDamageTime, false);
		FindAttackTarget(player);
	}
}
