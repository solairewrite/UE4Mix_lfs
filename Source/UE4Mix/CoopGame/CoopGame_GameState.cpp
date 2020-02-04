// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame_GameState.h"
#include "Net/UnrealNetwork.h"

void ACoopGame_GameState::OnRep_WaveState(EWaveState OldState)
{
	WaveStateChanged(WaveState, OldState);
}

void ACoopGame_GameState::SetState(EWaveState NewState)
{
	if (Role == ROLE_Authority)
	{
		EWaveState OldState = WaveState;
		WaveState = NewState;
		OnRep_WaveState(OldState);
	}
}

void ACoopGame_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACoopGame_GameState, WaveState);
}
