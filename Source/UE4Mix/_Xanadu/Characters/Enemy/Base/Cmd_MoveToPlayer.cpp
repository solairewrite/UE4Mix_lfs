// Fill out your copyright notice in the Description page of Project Settings.


#include "Cmd_MoveToPlayer.h"
#include "AICharacterBase.h"

void ACmd_MoveToPlayer::StartCommand()
{
	Super::StartCommand();

	AAICharacterBase* character = GetCharacter<AAICharacterBase>();
	if (character)
	{
		character->MoveToPlayer();
	}
}
