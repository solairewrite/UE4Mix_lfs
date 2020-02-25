// Fill out your copyright notice in the Description page of Project Settings.


#include "Cmd_Melee.h"
#include "../Base/AIControllerBase.h"

void ACmd_Melee::StartCommand()
{
	Super::StartCommand();

	if (OwnerController)
	{
		OwnerController->PlayAnim(TEXT("Melee"));
	}
}
