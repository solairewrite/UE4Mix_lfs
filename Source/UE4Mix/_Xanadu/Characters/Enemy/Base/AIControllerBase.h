// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

class ACommandManager;

/**
 * 
 */
UCLASS()
class UE4MIX_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
protected:
	ACommandManager* CommandManager;

public:
	void StartCommand();
};
