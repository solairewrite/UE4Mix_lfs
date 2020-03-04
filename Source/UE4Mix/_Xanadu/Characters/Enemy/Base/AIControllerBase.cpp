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

extern TAutoConsoleVariable<int32> CVARDebugLevel;

AAIControllerBase::AAIControllerBase()
{
	AIState = EAIState::Idle;
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	InitAnimManager();

	InitCommandManager();
	// TODO 切换战斗状态
	//StartCommand();

	StartIdle();
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
		break;
	case EAIState::Patrol:
		break;
	case EAIState::Alert:
		break;
	case EAIState::SearchTarget:
		break;
	case EAIState::Attack:
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
	AAICharacterBase* character = GetCharacter<AAICharacterBase>();
	if (!character)
	{
		return;
	}
	float tMin = character->GetIdleTimeRangeMin();
	float tMax = character->GetIdleTimeRangeMax();
	tMin = FMath::Max(0.1f, tMin);
	tMax = FMath::Max(tMin, tMax);
	float tIdleTime = UKismetMathLibrary::RandomFloatInRange(tMin, tMax);
	// 定时结束Idle状态
	GetWorldTimerManager().SetTimer(TH_FinishIdle, this, &AAIControllerBase::FinishIdle, tIdleTime, false);
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

bool AAIControllerBase::bIsNormalState()
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
