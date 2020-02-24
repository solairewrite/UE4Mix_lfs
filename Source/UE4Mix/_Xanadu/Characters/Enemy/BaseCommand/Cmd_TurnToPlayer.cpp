// Fill out your copyright notice in the Description page of Project Settings.


#include "Cmd_TurnToPlayer.h"
#include "../Base/AICharacterBase.h"

void ACmd_TurnToPlayer::StartCommand()
{
	Super::StartCommand();

	AAICharacterBase* character = GetCharacter<AAICharacterBase>();
	if (character)
	{
		character->TurnToPlayer();
	}
}
