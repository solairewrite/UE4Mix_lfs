// Fill out your copyright notice in the Description page of Project Settings.


#include "AICommand.h"
#include "AIAction.h"

// Sets default values
AAICommand::AAICommand()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAICommand::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAICommand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAICommand::InitCommand(AAIControllerBase* inController, AAIAction* inAction, EDoWhatOnLastCommandFail inDoWhatOnLastCommandFail)
{
	OwnerController = inController;
	OwnerAction = inAction;

	DoWhatOnLastCommandFail = inDoWhatOnLastCommandFail;
}

void AAICommand::StartCommand()
{
	SetCommandState(ECommandState::Doing);
}

void AAICommand::SetCommandState(ECommandState inState)
{
	if (CommandState == inState)
	{
		return;
	}

	CommandState = inState;
	// switch自动补全:输入switch后双击Tab
	switch (CommandState)
	{
	case ECommandState::None:
		break;
	case ECommandState::Doing:
		break;
	case ECommandState::Success:
		CommandSuccess();
		break;
	case ECommandState::Fail:
		CommandFail();
		break;
	default:
		break;
	}
}

ECommandState AAICommand::GetCommandState()
{
	return CommandState;
}

EDoWhatOnLastCommandFail AAICommand::GetDoWhatOnLastCommandFail()
{
	return DoWhatOnLastCommandFail;
}

void AAICommand::CommandSuccess()
{
	if (OwnerAction)
	{
		OwnerAction->OnCommandSuccess(this);
	}
}

void AAICommand::CommandFail()
{
	if (OwnerAction)
	{
		OwnerAction->OnCommandFail(this);
	}
}

