// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Fox.h"
#include "../Base/CommandManager.h"

AAIController_Fox::AAIController_Fox()
{
	CommandManagerClass = ACommandManager::StaticClass();
}

void AAIController_Fox::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, TEXT("AIController_Fox C++打印到屏幕"));
}
