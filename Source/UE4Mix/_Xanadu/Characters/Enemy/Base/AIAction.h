// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIAction.generated.h"

class AAIControllerBase;
class ACommandManager;
class AAICommand;

UENUM()
enum class EActionState :uint8
{
	None,
	Doing,
	Success,
	Fail,
};

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

protected:
	AAIControllerBase* Controller;

	ACommandManager* CommandManager;

	EActionState ActionStatus;

	TArray<AAICommand*> CommandArr;
	AAICommand* CurrentCommand;
	AAICommand* LastCommand;
	int CurrentCommandIndex;

public:
	void SetCommandArray();

	void StartCommand();

	void StartNextCommand();

	void GetNextCommand();

	void OnCommandSuccess();

	void OnCommandFail();

	void ActionSuccess();

	void ActionFail();

	EActionState GetActionState();
};
