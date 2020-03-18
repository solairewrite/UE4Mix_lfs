// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chest.generated.h"

class AInventory;
class UStaticMeshComponent;
class USceneComponent;

/**
 * 宝箱基类
 */
UCLASS()
class UE4MIX_API AChest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 宝箱内的宝物数组
	UPROPERTY(EditAnywhere, Category = "Chest")
		TArray<TSubclassOf<AInventory>> InventoryClassArr;

	// 是否给出所有的宝物
	UPROPERTY(EditAnywhere, Category = "Chest")
		bool bGiveAllInventory;

	// 是否随机生成宝物
	UPROPERTY(EditAnywhere, Category = "Chest")
		bool bGiveRandomInventory;

	// 是否需要钥匙来打开宝箱
	UPROPERTY(EditAnywhere, Category = "Chest")
		bool bNeedKey;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* ChestBase;

	// 宝箱盖子
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* ChestLid;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* SpawnPoint;

protected:
	UFUNCTION(BlueprintCallable, Category = "Chest")
		virtual bool IsCanOpenChest();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Chest")
		void OpenChest();

	// 宝箱打开后的回调函数
	UFUNCTION(BlueprintCallable, Category = "Chest")
		virtual void OnChestOpened();

	// 给出宝藏
	virtual void DropInventory();

};
