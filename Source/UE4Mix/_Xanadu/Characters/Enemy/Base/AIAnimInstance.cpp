// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAnimInstance.h"
#include "AICharacterBase.h"

void UAIAnimInstance::AnimNotify_Melee(UAnimNotify* inNotify)
{
	AAICharacterBase* character = GetCharacter<AAICharacterBase>();
	if (character)
	{
		character->OnMelee();
	}
}

void UAIAnimInstance::AnimNotify_DeathAnimEnd(UAnimNotify* inNotify)
{
	AAICharacterBase* character = GetCharacter<AAICharacterBase>();
	if (character)
	{
		character->OnDeathAnimEnd();
	}
}
