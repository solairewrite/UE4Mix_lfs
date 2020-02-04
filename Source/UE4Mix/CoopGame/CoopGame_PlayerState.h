// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CoopGame_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UE4MIX_API ACoopGame_PlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
		void AddScore(float ScoreDelta);
};
