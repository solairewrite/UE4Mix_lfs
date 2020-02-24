// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Characters/Enemy/Base/AICommand.h"
#include "Cmd_TurnToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UE4MIX_API ACmd_TurnToPlayer : public AAICommand
{
	GENERATED_BODY()
	
public:
	virtual void StartCommand();
};
