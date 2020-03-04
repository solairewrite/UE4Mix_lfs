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

protected:
	// 创建指定类型的Action,加到ActionArr中
	void AddAction(TSubclassOf<AAIAction> inActionClass);

	int GetNextActionIndex();
	AAIAction* GetNextAction();

	// 对所有的Action进行垃圾回收
	void GC_Actions();

};
