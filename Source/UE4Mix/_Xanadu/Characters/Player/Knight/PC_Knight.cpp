// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Knight.h"

APC_Knight::APC_Knight()
{

}

void APC_Knight::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, TEXT("骑士PC"));
}
