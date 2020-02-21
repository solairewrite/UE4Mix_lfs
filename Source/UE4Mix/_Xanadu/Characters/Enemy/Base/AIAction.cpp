// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAction.h"
#include "AICommand.h"
#include "CommandManager.h"

// Sets default values
AAIAction::AAIAction()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIAction::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAIAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIAction::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (AAICommand* cmd : CommandArr)
	{
		if (cmd)
		{
			cmd->Destroy();
		}
	}
}

void AAIAction::InitAction(AAIControllerBase* inController, ACommandManager* inManager)
{
	OwnerController = inController;
	CommandManager = inManager;
}

void AAIAction::StartAction()
{
	SetCommandArray();

	if (CommandArr.Num() > 0 && CommandArr[0])
	{
		CurrentCommandIndex = 0;
		CurrentCommand = CommandArr[0];
		CurrentCommand->StartCommand();

		SetActionState(EActionState::Doing);
	}
	else
	{
		SetActionState(EActionState::Success);
	}
}

void AAIAction::SetCommandArray()
{
	for (AAICommand* cmd : CommandArr)
	{
		if (cmd)
		{
			cmd->Destroy();
		}
	}
	CommandArr.Empty();
}

void AAIAction::SetActionState(EActionState inState)
{
	if (ActionState == inState)
	{
		return;
	}

	ActionState = inState;
	switch (ActionState)
	{
	case EActionState::None:
		break;
	case EActionState::Doing:
		break;
	case EActionState::Success:
		ActionSuccess();
		break;
	case EActionState::Fail:
		ActionFail();
		break;
	default:
		break;
	}
}

EActionState AAIAction::GetActionState()
{
	return ActionState;
}

void AAIAction::StartNextCommand()
{
	AAICommand* nextCommand = GetNextCommand();
	if (nextCommand)
	{
		LastCommand = CurrentCommand;

		CurrentCommandIndex++;
		CurrentCommand = nextCommand;
		CurrentCommand->StartCommand();
	}
	else
	{
		SetActionState(EActionState::Success);
	}
}

void AAIAction::OnCommandSuccess(AAICommand* inCommand)
{
	AAICommand* nextCommand = GetNextCommand();
	if (nextCommand)
	{
		StartNextCommand();
	}
	else
	{
		SetActionState(EActionState::Success);
	}
}

void AAIAction::OnCommandFail(AAICommand* inCommand)
{
	AAICommand* nextCommand = GetNextCommand();
	if (nextCommand)
	{
		EDoWhatOnLastCommandFail DoWhat = nextCommand->GetDoWhatOnLastCommandFail();
		switch (DoWhat)
		{
		case EDoWhatOnLastCommandFail::Execute:
			StartNextCommand();
			break;
		case EDoWhatOnLastCommandFail::ReDoLastCommand:
			CurrentCommand->StartCommand();
			break;
		case EDoWhatOnLastCommandFail::Skip:
			CurrentCommandIndex++;
			StartNextCommand();
			break;
		case EDoWhatOnLastCommandFail::ActionFail:
			SetActionState(EActionState::Fail);
			break;
		default:
			break;
		}
	}
	else
	{
		SetActionState(EActionState::Fail);
	}
}

AAICommand* AAIAction::GetNextCommand()
{
	if (CurrentCommandIndex < CommandArr.Num() - 1)
	{
		int nextIndex = CurrentCommandIndex + 1;
		if (CommandArr[nextIndex])
		{
			return CommandArr[nextIndex];
		}
	}
	return nullptr;
}

void AAIAction::ActionSuccess()
{
	if (CommandManager)
	{
		CommandManager->OnActionSuccess(this);
	}
}

void AAIAction::ActionFail()
{
	if (CommandManager)
	{
		CommandManager->OnActionFail(this);
	}
}

