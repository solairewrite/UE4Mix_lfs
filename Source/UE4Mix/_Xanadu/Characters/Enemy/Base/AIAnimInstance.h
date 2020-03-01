// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AIAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class UE4MIX_API UAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	template<class T>
	T* GetCharacter();

	// 动画通知:
	// 1,新建 MyAnimInst 继承自 UAIAnimInstance(动画蓝图)
	// 2,新建动画蓝图 ABP_My 继承自 MyAnimInst
	// 3,在 ABP_My 播放的动画中加入通知 Melee
	// 4,MyAnimInst 代码中加入动画通知函数 AnimNotify_Melee,必须以 "AnimNotify" 开头
	UFUNCTION(BlueprintCallable, Category = "Anim")
		virtual void AnimNotify_Melee(UAnimNotify* inNotify);

	UFUNCTION(BlueprintCallable, Category = "Anim")
		virtual void AnimNotify_DeathAnimEnd(UAnimNotify* inNotify);
};

template<class T>
T* UAIAnimInstance::GetCharacter()
{
	T* character = Cast<T>(TryGetPawnOwner());
	return character;
}
