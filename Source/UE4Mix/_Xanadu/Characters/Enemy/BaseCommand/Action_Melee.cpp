// Fill out your copyright notice in the Description page of Project Settings.


#include "Action_Melee.h"
#include "Cmd_MoveToPlayer.h"
#include "Cmd_TurnToPlayer.h"

void AAction_Melee::SetCommandArray()
{
	ACmd_MoveToPlayer* moveToPlayer = AddCommand<ACmd_MoveToPlayer>(EDoWhatOnLastCommandFail::Execute);
	
	ACmd_TurnToPlayer* turnToPlayer = AddCommand<ACmd_TurnToPlayer>(EDoWhatOnLastCommandFail::Execute);
}
