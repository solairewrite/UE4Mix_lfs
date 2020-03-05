// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Characters/Player/Base/PlayerControllerBase.h"
#include "PC_Knight.generated.h"

/**
 * 
 */
UCLASS()
class UE4MIX_API APC_Knight : public APlayerControllerBase
{
	GENERATED_BODY()
	
public:
	APC_Knight();

protected:
	virtual void BeginPlay() override;
};
