// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommandManager.generated.h"

class AAIControllerBase;
class AAIAction;

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

protected:
	AAIControllerBase* Controller;

	TArray<AAIAction*> ActionArr;
	AAIAction* CurrentAction;
	int CurrentActionIndex;

public:
	void SetActionArray();

	void StartAction();

	void StartNextAction();

	void GetNextAction();

	void OnActionSuccess();

	void OnActionFail();
};
