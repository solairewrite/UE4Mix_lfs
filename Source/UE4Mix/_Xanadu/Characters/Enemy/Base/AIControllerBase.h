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
	TSubclassOf<ACommandManager> CommandManagerClass;
	ACommandManager* CommandManager;

	AAIAnimManager* AnimManager;

public:
	virtual void BeginPlay() override;

	void InitAnimManager();

	void InitCommandManager();

	void StartCommand();

	// PlayAnim()前必须调用PrepareForPlayAnim()
	void PrepareForPlayAnim(AAICommand* inCommand);

	void PlayAnim(FName inAnimName);

	void OnPlayAnimSuccess(FName inAnimName);

	void OnPlayAnimFail(FName inAnimName);
};
