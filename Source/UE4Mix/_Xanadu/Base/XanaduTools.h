// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XanaduTypes.h"

/**
 *
 */
class UE4MIX_API XanaduTools
{
public:
	XanaduTools();
	~XanaduTools();

	static void LogScreen(const FString& inStr, int8 DebugLevel = 1, uint64 Key = -1, FColor DisplayColor = FColor::Yellow, float TimeToDisplay = 2.0f);

	static FString EAIStateToString(EAIState inState);
};
