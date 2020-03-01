// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInst_Knight.h"
#include "PlayerCharacter_Knight.h"

void UAnimInst_Knight::AnimNotify_OnMelee1(UAnimNotify* inNotify)
{
	APlayerCharacter_Knight* character = GetCharacter<APlayerCharacter_Knight>();
	if (character)
	{
		character->OnMelee1();
	}
}

void UAnimInst_Knight::AnimNotify_OnMelee2(UAnimNotify* inNotify)
{
	APlayerCharacter_Knight* character = GetCharacter<APlayerCharacter_Knight>();
	if (character)
	{
		character->OnMelee2();
	}
}

void UAnimInst_Knight::AnimNotify_OnMelee3(UAnimNotify* inNotify)
{
	APlayerCharacter_Knight* character = GetCharacter<APlayerCharacter_Knight>();
	if (character)
	{
		character->OnMelee3();
	}
}

void UAnimInst_Knight::AnimNotify_SaveAttack(UAnimNotify* inNotify)
{
	APlayerCharacter_Knight* character = GetCharacter<APlayerCharacter_Knight>();
	if (character)
	{
		character->OnSaveAttack();
	}
}

void UAnimInst_Knight::AnimNotify_ResetCombo(UAnimNotify* inNotify)
{
	APlayerCharacter_Knight* character = GetCharacter<APlayerCharacter_Knight>();
	if (character)
	{
		character->OnResetCombo();
	}
}
