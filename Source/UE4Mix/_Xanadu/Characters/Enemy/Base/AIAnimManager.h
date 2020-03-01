// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIAnimManager.generated.h"

class AAIControllerBase;
class AAICharacterBase;
class AAICommand;

UCLASS()
class UE4MIX_API AAIAnimManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAIAnimManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	AAIControllerBase* OwnerController;

	// 要顺序依次播放的动画队列
	TArray<FName> AnimQueue;

	// 当前动画队列中播放的动画索引
	int32 CurrentAnimIndex;

	AAICommand* CurrCommand;

	// 暂停当前命令的动画名
	FName PauseCommandAnimName;

public:
	void Init(AAIControllerBase* inController);

	void InitCommand(AAICommand* inCommand);

	// 顺序播放动画队列中的动画,直到最后一个动画播放完,当前命令成功
	void PlayAnim(FName inAnimName);

	// 立刻播放动画,暂停当前命令,并在动画结束之后,重新开始命令
	void PlayAnimImmediately(FName inAnimName);

	void OnPlayAnimSuccess(FName inAnimMane);

protected:
	AAICharacterBase* GetCharacter();

	template<class T>
	T* GetCharacter()
	{
		return Cast<T>(GetCharacter());
	}

	void PlayAnimInQueue(int32 inIndex);

	void ResetAnimManager();

	void OnCommandSuccess();

	void OnCommandFail();
};
