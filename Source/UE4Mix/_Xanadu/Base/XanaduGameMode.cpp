// Fill out your copyright notice in the Description page of Project Settings.


#include "XanaduGameMode.h"
#include "_Xanadu/Characters/Player/Knight/PlayerCharacter_Knight.h"
#include "_Xanadu/Characters/Player/Knight/PC_Knight.h"

AXanaduGameMode::AXanaduGameMode()
{
	DefaultPawnClass = APlayerCharacter_Knight::StaticClass();
	PlayerControllerClass = APC_Knight::StaticClass();
}
