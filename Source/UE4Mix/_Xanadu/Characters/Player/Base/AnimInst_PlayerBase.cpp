// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInst_PlayerBase.h"
#include "PlayerCharacterBase.h"

void UAnimInst_PlayerBase::AnimNotify_Melee(UAnimNotify* inNotify)
{
	APlayerCharacterBase* character = GetCharacter<APlayerCharacterBase>();
	if (character)
	{
		character->OnMelee();
	}
}
