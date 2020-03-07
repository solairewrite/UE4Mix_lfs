// Fill out your copyright notice in the Description page of Project Settings.


#include "XanaduTools.h"
#include "_Xanadu/Base/XanaduTypes.h"
#include "Engine.h"

extern TAutoConsoleVariable<int32> CVARDebugLevel;

XanaduTools::XanaduTools()
{
}

XanaduTools::~XanaduTools()
{
}

void XanaduTools::LogScreen(const FString& inStr, int8 DebugLevel /*= 1*/, uint64 Key /*= 1*/, FColor DisplayColor /*= FColor::Yellow*/, float TimeToDisplay /*= 2.0f*/)
{
	if (CVARDebugLevel.GetValueOnGameThread() < DebugLevel)
	{
		return;
	}
	if (!GEngine)
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, DisplayColor, inStr);
}

FString XanaduTools::EAIStateToString(EAIState inState)
{
	switch (inState)
	{
	case EAIState::None:
		return TEXT("None");
		break;
	case EAIState::Escape:
		return TEXT("Escape");
		break;
	case EAIState::Sleep:
		return TEXT("Sleep");
		break;
	case EAIState::Rest:
		return TEXT("Rest");
		break;
	case EAIState::Idle:
		return TEXT("Idle");
		break;
	case EAIState::Patrol:
		return TEXT("Patrol");
		break;
	case EAIState::Alert:
		return TEXT("Alert");
		break;
	case EAIState::SearchTarget:
		return TEXT("SearchTarget");
		break;
	case EAIState::Attack:
		return TEXT("Attack");
		break;
	default:
		return TEXT("None");
		break;
	}
}
