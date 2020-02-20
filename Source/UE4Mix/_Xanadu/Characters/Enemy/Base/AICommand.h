// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AICommand.generated.h"

class AAIControllerBase;
class AAIAction;

UENUM()
enum class ECommandState :uint8
{
	None,
	Doing,
	Success,
	Fail,
};

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

protected:
	ECommandState CommandState;

	AAIControllerBase* Conrtoller;

	AAIAction* Action;

public:
	void InitCommand();

	void CommandSuccess();

	void CommandFail();

	ECommandState GetCommandState();
};
