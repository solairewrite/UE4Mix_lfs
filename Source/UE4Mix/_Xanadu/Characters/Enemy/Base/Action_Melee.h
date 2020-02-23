// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Characters/Enemy/Base/AIAction.h"
#include "Action_Melee.generated.h"

/**
 * 
 */
UCLASS()
class UE4MIX_API AAction_Melee : public AAIAction
{
	GENERATED_BODY()
	
public:
	virtual void SetCommandArray() override;
};
