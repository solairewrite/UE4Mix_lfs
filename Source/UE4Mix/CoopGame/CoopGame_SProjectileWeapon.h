// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoopGame/CoopGame_SWeapon.h"
#include "CoopGame_SProjectileWeapon.generated.h"

/**
 *
 */
UCLASS()
class UE4MIX_API ACoopGame_SProjectileWeapon : public ACoopGame_SWeapon
{
	GENERATED_BODY()

protected:

	virtual void Fire() override;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon")
		TSubclassOf<AActor> ProjectileClass;
};
