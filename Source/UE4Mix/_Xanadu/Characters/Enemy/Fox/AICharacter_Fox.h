// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Characters/Enemy/Base/AICharacterBase.h"
#include "AICharacter_Fox.generated.h"

class ACharacter;
class UNavigationPath;

/**
 *
 */
UCLASS()
class UE4MIX_API AAICharacter_Fox : public AAICharacterBase
{
	GENERATED_BODY()

public:
	AAICharacter_Fox();

	virtual bool CanPlayTakeHitAnim() override;

	virtual bool CanTakeImpulse() override;
};
