// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopGame_NormalBox.generated.h"

class UStaticMeshComponent;

UCLASS()
class UE4MIX_API ACoopGame_NormalBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACoopGame_NormalBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UStaticMeshComponent* MeshComp;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
