// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UBoxComponent;

// 声明无参代理
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenDoorSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCloseDoorSignature);

/**
 * 门基类,可以加载/卸载地图
 */
UCLASS()
class UE4MIX_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

protected:
	// 开关门触发器
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* OpenDoorTrigger;

	// 加载门前侧地图的触发器
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* LoadFrontMapTrigger;
	// 卸载门前侧地图的触发器
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* UnLoadFrontMapTrigger;

	// 加载门后侧地图的触发器
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* LoadBackMapTrigger;
	// 卸载门后侧地图的触发器
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UBoxComponent* UnLoadBackMapTrigger;

	// 开门时是否加载地图
	UPROPERTY(EditAnywhere, Category = "Door")
		bool bLoadLevelOnOpenDoor;
	// 关门时是否卸载地图
	UPROPERTY(EditAnywhere, Category = "Door")
		bool bUnLoadLevelOnCloseDoor;

	// 门前侧的地图名称
	UPROPERTY(EditInstanceOnly, Category = "Door")
		FName FrontMapName;

	// 门后侧的地图名称
	UPROPERTY(EditInstanceOnly, Category = "Door")
		FName BackMapName;

	// 开门代理
	UPROPERTY(BlueprintAssignable, Category = "Door")
		FOpenDoorSignature OnDoorOpenedDelegate;
	// 关门代理
	UPROPERTY(BlueprintAssignable, Category = "Door")
		FCloseDoorSignature OnDoorClosedDelegate;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Door")
		void OpenDoor();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Door")
		void CloseDoor();

protected:
	// 门被打开的回调函数,蓝图中调用,触发开门代理
	UFUNCTION(BlueprintCallable, Category = "Door")
		void OnDoorOpened();
	// 门被关闭的回调函数,蓝图中调用,触发开门代理
	UFUNCTION(BlueprintCallable, Category = "Door")
		void OnDoorClosed();
};
