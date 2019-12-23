// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LiftCube_Cube.generated.h"

class UStaticMeshComponent;

UCLASS()
class UE4MIX_API ALiftCube_Cube : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALiftCube_Cube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Cube")
		UStaticMeshComponent* CubeComp;

	UPROPERTY(EditDefaultsOnly, Category = "Cube")
		float Size;

	UPROPERTY(EditDefaultsOnly, Category = "Cube")
		FVector Scale3D;

	UPROPERTY(EditDefaultsOnly, Category = "Cube")
		float MoveSpeed; // 多久升到最高
	float TargetHeight; // 上升或下降的目标高度

	UPROPERTY(EditDefaultsOnly, Category = "Cube")
		UMaterialInterface* DefaultMat;

	UPROPERTY(EditDefaultsOnly, Category = "Cube")
		UMaterialInterface* RiseMat;

	APawn* Player;
	
	void SetDefaultMat();
	void SetRiseMat();

	APawn* GetPlayer();

public:
	void Rise(float RiseHeight);
	void Down();
	bool IsRise();

	float GetSize();

	int DistanceToPlayerCubeUnit();

	virtual void Destroyed();
};
