// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAnimManager.h"
#include "AIControllerBase.h"
#include "AICharacterBase.h"
#include "AICommand.h"

// Sets default values
AAIAnimManager::AAIAnimManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIAnimManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAIAnimManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIAnimManager::Init(AAIControllerBase* inController)
{
	OwnerController = inController;
	ResetAnimManager();
}

void AAIAnimManager::InitCommand(AAICommand* inCommand)
{
	ResetAnimManager();
	CurrCommand = inCommand;
}

void AAIAnimManager::PlayAnim(FName inAnimName)
{
	AnimQueue.Add(inAnimName);

	if (AnimQueue.Num() == 0)
	{
		CurrentAnimIndex = 0;
		PlayAnimInQueue(CurrentAnimIndex);
	}
}

AAICharacterBase* AAIAnimManager::GetCharacter()
{
	if (OwnerController)
	{
		return Cast<AAICharacterBase>(OwnerController->GetCharacter());
	}
	return nullptr;
}

void AAIAnimManager::PlayAnimInQueue(int32 inIndex)
{
	AAICharacterBase* character = GetCharacter();
	if (!character)
	{
		OnCommandFail();
		return;
	}

	if (inIndex >= AnimQueue.Num())
	{
		OnCommandFail();
		return;
	}
	FName animName = AnimQueue[inIndex];
	if (animName.IsNone())
	{
		OnCommandFail();
		return;
	}

	character->PlayAnim(animName);
}

void AAIAnimManager::ResetAnimManager()
{
	CurrCommand = nullptr;
	AnimQueue.Empty();
	CurrentAnimIndex = 0;
}

void AAIAnimManager::OnCommandSuccess()
{
	CurrCommand->CommandSuccess();
	ResetAnimManager();
}

void AAIAnimManager::OnCommandFail()
{
	CurrCommand->CommandFail();
	ResetAnimManager();
}

