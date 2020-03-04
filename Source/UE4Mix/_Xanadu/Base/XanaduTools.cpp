// Fill out your copyright notice in the Description page of Project Settings.


#include "XanaduTools.h"

XanaduTools::XanaduTools()
{
}

XanaduTools::~XanaduTools()
{
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
