﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

UCLASS()
class UE4MIX_API ASpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UStaticMeshComponent* Point_Visualizer;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UArrowComponent* ArrowComp;

	void CreatePointVisualizer();

	void CreateArrow();
};
