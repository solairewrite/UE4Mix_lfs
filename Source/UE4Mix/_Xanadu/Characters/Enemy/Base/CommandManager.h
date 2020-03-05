// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommandManager.generated.h"

class AAIControllerBase;
class AAIAction;

/**
 * AI命令管理器
 * 控制战斗状态下的AI行为
 */
UCLASS()
class UE4MIX_API ACommandManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACommandManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

protected:
	AAIControllerBase* OwnerController;

	TArray<AAIAction*> ActionArr;
	AAIAction* CurrentAction;
	int CurrentActionIndex;

public:
	void Init(AAIControllerBase* inController);

	// 设置Action列表,开启第一个Action
	void StartAction();

	// StartAction()中调用,将所有Action存入ActionArr
	virtual void SetActionArray();

	void StartNextAction();

	void OnActionSuccess(AAIAction* inAction);

	void OnActionFail(AAIAction* inAction);

	void OnActionAbort(AAIAction* inAction);

	// 停止所有Action,如丢失玩家或死亡时调用
	void StopAction();

protected:
	// 是否已经设置Action数组,防止每次开启命令时,重复设置
	bool bHasSetActionArr;

	// 创建指定类型的Action,加到ActionArr中
	void AddAction(TSubclassOf<AAIAction> inActionClass);

	int GetNextActionIndex();
	AAIAction* GetNextAction();

	// 对所有的Action进行垃圾回收
	void GC_Actions();

	// 重置所有Action状态,并不删除Action
	void ResetCommandManager();

};
