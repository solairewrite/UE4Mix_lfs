// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "CommandManager.h"
#include "Engine/World.h"

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	InitCommandManager();
	StartCommand();
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
