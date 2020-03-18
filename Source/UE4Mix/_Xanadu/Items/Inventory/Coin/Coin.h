// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Items/Inventory/Inventory.h"
#include "Coin.generated.h"

class UStaticMeshComponent;

/**
 * 金币基类
 */
UCLASS()
class UE4MIX_API ACoin : public AInventory
{
	GENERATED_BODY()

public:
	ACoin();

protected:
	UPROPERTY(EditAnywhere, Category = "Coin")
		int Value;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* CoinMesh;

public:
	int GetValue() { return Value; }
	void SetValue(int inValue) { Value = FMath::Max(0, inValue); }
};
