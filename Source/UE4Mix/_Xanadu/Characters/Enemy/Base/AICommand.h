// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIControllerBase.h"
#include "_Xanadu/Base/XanaduTypes.h"
#include "AICommand.generated.h"

class AAIAction;

/**
 * AI基础命令的基类,如跑向玩家,转向玩家,攻击等
 * 每个AI行为(AIAction)都由一些列基础命令组成,如近战攻击包含跑向玩家,转向玩家,攻击
 * 当命令成功或失败后,触发AIAction执行一下个命令或停止
 */
UCLASS()
class UE4MIX_API AAICommand : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAICommand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	ECommandState CommandState;

	EDoWhatOnLastCommandFail DoWhatOnLastCommandFail;

	// 拥有这个命令的AIController
	AAIControllerBase* OwnerController;

	template<class T>
	T* GetCharacter();

	// 拥有这个命令的AIAction
	AAIAction* OwnerAction;

public:
	// 初始化命令,在Action中注册
	void InitCommand(AAIControllerBase* inController, AAIAction* inAction, EDoWhatOnLastCommandFail inDoWhatOnLastCommandFail);

	// 设置Character的当前Command,并应该调用相应的函数
	virtual void StartCommand();

	virtual void ReDoCommand();

	// 设置命令状态,触发新状态对应的函数
	void SetCommandState(ECommandState inState);

	ECommandState GetCommandState();

	EDoWhatOnLastCommandFail GetDoWhatOnLastCommandFail();

	// 命令成功,触发Action执行下一条命令
	virtual void CommandSuccess();

	// 命令失败,触发Action根据需要执行下一条命令
	virtual void CommandFail();

	virtual void CommandPause();

	virtual void CommandContinue();

};

template<class T>
T* AAICommand::GetCharacter()
{
	if (OwnerController)
	{
		//return Cast<T>(OwnerController->GetCharacter());
		return OwnerController->GetCharacter<T>();
	}
	return nullptr;
}
