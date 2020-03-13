// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "_Xanadu/Items/Light/LightBase.h"
#include "FireBase.generated.h"

/**
 * 火焰基类
 */
UCLASS()
class UE4MIX_API AFireBase : public ALightBase
{
	GENERATED_BODY()

public:
	AFireBase();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* FirePSC;

	//UPROPERTY(EditAnywhere, Category = "Fire")
	//	FVector MeshScale;

	//UPROPERTY(EditAnywhere, Category = "Fire")
	//	FVector FirePSCScale;

	//UPROPERTY(EditAnywhere, Category = "Fire")
	//	FVector FirePSCOffset;

public:
	// 继承光照基类的开灯函数,在火焰类中等同于点亮火焰
	virtual void TurnOnLight() override;

	virtual void TurnOffLight() override;

	// 火焰类特有的点亮火焰功能
	virtual void TurnOnFire();

	virtual void TurnOffFire();
};
