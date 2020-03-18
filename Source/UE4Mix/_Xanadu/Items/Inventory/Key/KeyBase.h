// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Items/Inventory/Inventory.h"
#include "KeyBase.generated.h"

/**
 * 钥匙基类
 */
UCLASS()
class UE4MIX_API AKeyBase : public AInventory
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
