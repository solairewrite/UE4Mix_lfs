// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter_Fox.h"
#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController_Fox.h"


AAICharacter_Fox::AAICharacter_Fox()
{
	AIControllerClass = AAIController_Fox::StaticClass();
}

bool AAICharacter_Fox::CanPlayTakeHitAnim()
{
	return true;
}

