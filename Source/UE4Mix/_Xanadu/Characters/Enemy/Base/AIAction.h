// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIAction.generated.h"

class AAIControllerBase;
class ACommandManager;
class AAICommand;
enum class EDoWhatOnLastCommandFail :uint8;

UENUM()
enum class EActionState :uint8
{
	None,
	Doing,
	Success,
	Fail,
};

/**
 * AI行为,由一些列AICommand组成
 * AI的每个技能都是一个AI行为,如释放近战攻击
 * 命令管理器控制一系列AI行为
 */
UCLASS()
class UE4MIX_API AAIAction : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAIAction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

protected:
	AAIControllerBase* OwnerController;

	ACommandManager* CommandManager;

	EActionState ActionState;

	TArray<AAICommand*> CommandArr;
	AAICommand* CurrentCommand;
	AAICommand* LastCommand;
	int CurrentCommandIndex;

public:
	// 初始化Action,并在命令管理器中注册
	void InitAction(AAIControllerBase* inController, ACommandManager* inManager);

	// 设置命令列表,开启第一个命令
	void StartAction();

	// StartAction()中调用,将Action所需的所有命令存入CommandArr
	virtual void SetCommandArray();

	// 设置Action状态,触发该状态对应的函数
	void SetActionState(EActionState inState);

	EActionState GetActionState();

	void StartNextCommand();

	void OnCommandSuccess(AAICommand* inCommand);

	void OnCommandFail(AAICommand* inCommand);


protected:
	// 创建命令,并加入CommandArr
	// 如果命令需要传参,需要额外调用函数
	template<class T>
	T* AddCommand(EDoWhatOnLastCommandFail inDoWhatOnLastCommandFail);

	AAICommand* GetNextCommand();

	virtual void ActionSuccess();

	virtual void ActionFail();

};

template<class T>
T* AAIAction::AddCommand(EDoWhatOnLastCommandFail inDoWhatOnLastCommandFail)
{
	static_assert(std::is_base_of<AAICommand, T>::value, "T应该是AAICommand的子类");

	FActorSpawnParameters spawnPara;
	spawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AAICommand* cmd = GetWorld()->SpawnActor<AAICommand>(T::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, spawnPara);

	if (cmd)
	{
		cmd->InitCommand(OwnerController, this, inDoWhatOnLastCommandFail);
		CommandArr.Add(cmd);
	}

	return Cast<T>(cmd);
}
