// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Characters/Player/Base/PlayerCharacterBase.h"
#include "PlayerCharacter_Knight.generated.h"

/**
 *
 */
UCLASS()
class UE4MIX_API APlayerCharacter_Knight : public APlayerCharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter_Knight();

protected:
	// 近战
	static const int8 MaxMeleeComboCount = 3;
	// 当前近战连击索引
	int8 CurrMeleeComboIndex;
	// 是否在触发下一次连击的动画通知之前,已经下达了连击命令
	bool bHasSaveMeleeCombo;

	UPROPERTY(EditDefaultsOnly, Category = "Anim")
		UAnimMontage* AnimMontage_MeleeArr[MaxMeleeComboCount];

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float MeleeDamageArr[MaxMeleeComboCount];

public:
	// 近战动画通知
	virtual void OnMelee() override;
	void OnMelee1();
	void OnMelee2();
	void OnMelee3();
	// 近战动画的攻击部分结束时触发
	// 如果在这段时间之内按下左键,则触发连击
	void OnSaveAttack();
	// 近战动画快结束时触发,重置近战状态
	void OnResetCombo();

protected:
	// 按下左键触发带有连击功能的近战,最多连击3次
	virtual void Melee() override;

	void DoMeleeDamage(float inDamage);
};
