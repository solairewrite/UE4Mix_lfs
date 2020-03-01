// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "CommandManager.h"
#include "Engine/World.h"
#include "AIAnimManager.h"
#include "AICommand.h"

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

	InitAnimManager();

	InitCommandManager();
	StartCommand();
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
