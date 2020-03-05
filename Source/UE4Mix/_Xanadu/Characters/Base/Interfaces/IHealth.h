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
	// 检测是否实现接口的两种方法:
	// 1, if (tActor->GetClass()->ImplementsInterface(UIHealth::StaticClass()))
	// 2, IIHealth* tHealthActor = Cast<IIHealth>(tActor);
	//	  if(tHealthActor)

	// BlueprintNativeEvent 标记的函数,既可以用蓝图实现,也可以用C++实现
	// 如果同时实现,蓝图覆盖C++
	// C++的实现函数名要加上"_Implementation"
	// 调用接口时,要加上"Execute前缀",第一个参数是实现接口的对象,后面的参数依次为对应的参数
	// float damage = IIHealth::Execute_GetHealth(tActor);
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Health")
	//	float GetHealth();
	//virtual float GetHealth_Implementation();

	virtual float GetHealth();

	// 普通函数可以直接调用接口
	// bIsAI = tHealthActor->IsAI();
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
