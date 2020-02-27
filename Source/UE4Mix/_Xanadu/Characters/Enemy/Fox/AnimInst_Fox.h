// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Characters/Enemy/Base/AIAnimInstance.h"
#include "AnimInst_Fox.generated.h"

/**
 * 
 */
UCLASS()
class UE4MIX_API UAnimInst_Fox : public UAIAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void AnimNotify_Melee(UAnimNotify* inNotify) override;
};
