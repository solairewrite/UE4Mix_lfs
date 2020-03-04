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

	static FString EAIStateToString(EAIState inState);
};
