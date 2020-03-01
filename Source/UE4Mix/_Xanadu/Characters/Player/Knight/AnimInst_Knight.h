// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Characters/Player/Base/AnimInst_PlayerBase.h"
#include "AnimInst_Knight.generated.h"

/**
 * 玩家动画蓝图基类,负责将动画通知转发给Character
 */
UCLASS()
class UE4MIX_API UAnimInst_Knight : public UAnimInst_PlayerBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Anim")
		void AnimNotify_OnMelee1(UAnimNotify* inNotify);
	UFUNCTION(BlueprintCallable, Category = "Anim")
		void AnimNotify_OnMelee2(UAnimNotify* inNotify);
	UFUNCTION(BlueprintCallable, Category = "Anim")
		void AnimNotify_OnMelee3(UAnimNotify* inNotify);

	UFUNCTION(BlueprintCallable, Category = "Anim")
		void AnimNotify_SaveAttack(UAnimNotify* inNotify);

	UFUNCTION(BlueprintCallable, Category = "Anim")
		void AnimNotify_ResetCombo(UAnimNotify* inNotify);
};
