// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

class ACommandManager;
class AAIAnimManager;
class AAICommand;
enum class EAIState :uint8;
class UAIPerceptionComponent;

/**
 *
 */
UCLASS()
class UE4MIX_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	AAIControllerBase();

	virtual void BeginPlay() override;

protected:
	// 命令管理器的类
	TSubclassOf<ACommandManager> CommandManagerClass;
	ACommandManager* CommandManager;

	// 动画管理器
	AAIAnimManager* AnimManager;

public:
	void InitAnimManager();

	void InitCommandManager();

	void StartCommand();

	// PlayAnim()前必须调用PrepareForPlayAnim()
	void PrepareForPlayAnim(AAICommand* inCommand);

	void PlayAnim(FName inAnimName);

	void PlayAnimImmediately(FName inAnimName);

	void OnPlayAnimSuccess(FName inAnimName);

	void OnPlayAnimFail(FName inAnimName);

	// 对命令管理器进行垃圾回收
	void GC_CommandManager();
	// 对动画控制器进行垃圾回收
	void GC_AnimManager();

	template<class T>
	T* GetCharacter();

protected:
	// AI状态,Idle,攻击等
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AICharacter")
		EAIState AIState;

	// 状态改变的回调函数,由SetAIState()调用,不应该主动调用
	virtual void OnAIStateChanged(EAIState OldState, EAIState NewState);

	virtual void OnEnterAIState(EAIState inState);
	virtual void OnLeaveAIState(EAIState inState);

	void StartIdle(); // 开始Idle,主动调用
	virtual void OnEnterIdleState();
	virtual void FinishIdle(); // 结束Idle状态,主动调用
	FTimerHandle TH_FinishIdle;

	virtual void OnEnterPatrolState();

	// 在一个普通状态结束时,决定下一个普通状态
	virtual EAIState DecideNextNormalState();

public:
	EAIState GetAIState() { return AIState; }
	// 设置AI状态,并触发对应状态的回调函数
	void SetAIState(EAIState inNewState);

	// 是否是一般状态,如Idle,休息,睡眠等
	bool bIsNormalState();

	void FinishPatrol(); // 到达巡逻点,主动调用

// 感知系统
protected:
	UPROPERTY(VisibleAnywhere, Category = "AIPerception")
		UAIPerceptionComponent* AIPerceptionComp;

	UPROPERTY(EditDefaultsOnly, Category = "AIPerception")
		float AIPerception_SightRadius;
	// 丢失已被看到目标的距离
	UPROPERTY(EditDefaultsOnly, Category = "AIPerception")
		float AIPerception_LoseSightRadius;
	// 感知视角范围
	UPROPERTY(EditDefaultsOnly, Category = "AIPerception")
		float AIPerception_FOV;
};

template<class T>
T* AAIControllerBase::GetCharacter()
{
	ACharacter* tCharacter = AController::GetCharacter();
	return Cast<T>(tCharacter);
}
