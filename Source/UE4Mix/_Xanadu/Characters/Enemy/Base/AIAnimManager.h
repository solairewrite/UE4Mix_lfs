// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIAnimManager.generated.h"

class AAIControllerBase;
class AAICharacterBase;

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

protected:
	AAIControllerBase* OwnerController;

public:
	void Init(AAIControllerBase* inController);

	float PlayAnim(FName inAnimName);

protected:
	AAICharacterBase* GetCharacter();

	template<class T>
	T* GetCharacter()
	{
		return Cast<T>(GetCharacter());
	}
};
