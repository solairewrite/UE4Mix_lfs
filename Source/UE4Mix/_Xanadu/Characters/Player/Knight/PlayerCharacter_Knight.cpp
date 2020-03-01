// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter_Knight.h"

void APlayerCharacter_Knight::OnMelee1()
{
	DoMeleeDamage(MeleeDamageArr[0]);
}

void APlayerCharacter_Knight::OnMelee2()
{
	DoMeleeDamage(MeleeDamageArr[1]);
}

void APlayerCharacter_Knight::OnMelee3()
{
	DoMeleeDamage(MeleeDamageArr[2]);
}

void APlayerCharacter_Knight::OnSaveAttack()
{
	// 检测是否下达了连击命令
	if (!bHasSaveMeleeCombo)
	{
		return;
	}

	bHasSaveMeleeCombo = false;
	// 播放下一条连击动画
	if (AnimMontage_MeleeArr[CurrMeleeComboIndex])
	{
		PlayAnimMontage(AnimMontage_MeleeArr[CurrMeleeComboIndex]);
		CurrMeleeComboIndex++;
		if (CurrMeleeComboIndex >= MaxMeleeComboCount)
		{
			CurrMeleeComboIndex = 0;
		}
	}
}

void APlayerCharacter_Knight::OnResetCombo()
{
	bIsMeleeing = false;
	bHasSaveMeleeCombo = false;
	CurrMeleeComboIndex = 0;
}

void APlayerCharacter_Knight::Melee()
{
	// 如果已经在近战攻击中,只记录连击命令
	if (bIsMeleeing)
	{
		bHasSaveMeleeCombo = true;
		return;
	}

	bIsMeleeing = true;
	if (AnimMontage_MeleeArr[0])
	{
		PlayAnimMontage(AnimMontage_MeleeArr[0]);
		CurrMeleeComboIndex = 1;
	}
}

void APlayerCharacter_Knight::DoMeleeDamage(float inDamage)
{
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Yellow, FString::SanitizeFloat(inDamage));
}
