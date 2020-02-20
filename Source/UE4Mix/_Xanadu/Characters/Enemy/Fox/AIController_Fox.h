// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Characters/Enemy/Base/AIControllerBase.h"
#include "AIController_Fox.generated.h"

/**
 * 
 */
UCLASS()
class UE4MIX_API AAIController_Fox : public AAIControllerBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
};
