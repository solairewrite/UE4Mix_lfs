// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopGame_STrackerBot.h"
#include "CoopGame_CollideQueryActor.generated.h"

class USphereComponent;

UCLASS()
class UE4MIX_API ACoopGame_CollideQueryActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoopGame_CollideQueryActor();

	void Init(ACoopGame_STrackerBot* InBot);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USphereComponent* SphereComp;

	ACoopGame_STrackerBot* Bot;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
};
