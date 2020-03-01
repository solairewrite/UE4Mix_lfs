// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

class ACommandManager;
class AAIAnimManager;
class AAICommand;

/**
 * 
 */
UCLASS()
class UE4MIX_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
protected:
	// 命令管理器的类
	TSubclassOf<ACommandManager> CommandManagerClass;
	ACommandManager* CommandManager;

	// 动画管理器
	AAIAnimManager* AnimManager;

public:
	virtual void BeginPlay() override;

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
};
