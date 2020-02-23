// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandManager.h"
#include "AIAction.h"
#include "Engine/World.h"
#include "Action_Melee.h"

// Sets default values
ACommandManager::ACommandManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACommandManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACommandManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACommandManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (AAIAction* action : ActionArr)
	{
		if (action)
		{
			action->Destroy();
		}
	}
}

void ACommandManager::Init(AAIControllerBase* inController)
{
	OwnerController = inController;
}

void ACommandManager::StartAction()
{
	SetActionArray();

	if (ActionArr.Num() > 0 && ActionArr[0])
	{
		CurrentActionIndex = 0;
		CurrentAction = ActionArr[0];
		CurrentAction->StartAction();
	}
}

void ACommandManager::SetActionArray()
{
	AddAction(AAction_Melee::StaticClass());
}

void ACommandManager::StartNextAction()
{
	// 由于最后一个Action的下一个是第一个,所以索引不能单纯的++
	AAIAction* nextAction = GetNextAction();
	if (nextAction)
	{
		CurrentActionIndex = GetNextActionIndex();
		CurrentAction = nextAction;
		CurrentAction->StartAction();
	}
}

void ACommandManager::OnActionSuccess(AAIAction* inAction)
{
	StartNextAction();
}

void ACommandManager::OnActionFail(AAIAction* inAction)
{
	// 暂时Action失败也开始下一个Action
	StartNextAction();
}

void ACommandManager::AddAction(TSubclassOf<AAIAction> inActionClass)
{
	if (inActionClass == nullptr)
	{
		return;
	}

	FActorSpawnParameters spawnPara;
	spawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AAIAction* action = GetWorld()->SpawnActor<AAIAction>
		(inActionClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnPara);

	if (action)
	{
		action->InitAction(OwnerController, this);
		ActionArr.Add(action);
	}
}

int ACommandManager::GetNextActionIndex()
{
	if (CurrentActionIndex < 0)
	{
		return -1;
	}
	if (CurrentActionIndex < ActionArr.Num() - 1)
	{
		int nextIndex = CurrentActionIndex + 1;
		return nextIndex;
	}
	// 如果当前是最后一个,则返回第一个
	else
	{
		return 0;
	}
}

AAIAction* ACommandManager::GetNextAction()
{
	int nextIndex = GetNextActionIndex();
	if (nextIndex >= 0)
	{
		return ActionArr[nextIndex];
	}
	return nullptr;
}

