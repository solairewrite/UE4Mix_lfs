// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

/**
 *
 */
UCLASS()
class UE4MIX_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	template<class T>
	T* GetCharacter();
};

template<class T>
T* APlayerControllerBase::GetCharacter()
{
	ACharacter* tCharacter = AController::GetCharacter();
	return Cast<T>(tCharacter);
}
