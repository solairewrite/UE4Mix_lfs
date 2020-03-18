﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

class APlayerCharacterBase;

/**
 * 玩家道具的基类,包括武器,消耗品,掉落物品等
 */
UCLASS()
class UE4MIX_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnPickUpByPlayer(APlayerCharacterBase* inPlayer);

};
