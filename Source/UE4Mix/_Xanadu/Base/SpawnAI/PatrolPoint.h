// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPoint.generated.h"

UCLASS()
class UE4MIX_API APatrolPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APatrolPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* Point_Visualizer;

	// 显示两个路径点之间的路径
	// .build.cs中添加"CableComponent"
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UCableComponent* Path_Visualizer;

	UPROPERTY(EditInstanceOnly, Category = "Path")
		APatrolPoint* NextPoint;
};
