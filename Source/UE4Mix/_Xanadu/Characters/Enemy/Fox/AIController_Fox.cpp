// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_Fox.h"

void AAIController_Fox::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("AIController_Fox C++打印到屏幕"));
}
