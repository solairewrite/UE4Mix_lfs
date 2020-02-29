// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInst_PlayerBase.generated.h"

/**
 * 玩家动画蓝图基类
 * 加入动画通知的处理
 */
UCLASS()
class UE4MIX_API UAnimInst_PlayerBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Anim")
		virtual void AnimNotify_Melee(UAnimNotify* inNotify);

	template<class T>
	T* GetCharacter();
};

template<class T>
T* UAnimInst_PlayerBase::GetCharacter()
{
	T* character = Cast<T>(TryGetPawnOwner());
	return character;
}
