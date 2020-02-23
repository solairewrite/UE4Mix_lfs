// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Characters/Enemy/Base/AICommand.h"
#include "Cmd_Melee.generated.h"

/**
 * 
 */
UCLASS()
class UE4MIX_API ACmd_Melee : public AAICommand
{
	GENERATED_BODY()
	
public:
	virtual void StartCommand();
};
