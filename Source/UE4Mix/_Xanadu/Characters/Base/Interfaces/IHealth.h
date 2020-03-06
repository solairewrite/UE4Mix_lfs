// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHealth.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIHealth : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class UE4MIX_API IIHealth
{
	GENERATED_BODY()

public:
	// float health = IIHealth::Execute_GetHealth(tActor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
		float GetHealth();
	virtual float GetHealth_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
		float GetHealthMax();
	virtual float GetHealthMax_Implementation();

	virtual bool IsAI();

	// 被攻击回调,不处理掉血逻辑(血量组件处理)
	virtual void OnAttackBy(class AController* InstigatedBy, AActor* DamageCauser);

	// 血量降为0时的回调函数
	virtual void OnDead();

	// 播放受击动画
	virtual bool CanPlayTakeHitAnim();
	virtual void PlayTakeHitAnim();

	// 受到冲力
	virtual bool CanTakeImpulse();
	virtual void TakeImpulse(FVector inImpulseValue);
};
