// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBase.h"
#include "_Xanadu/Base/XanaduTools.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"

AFireBase::AFireBase()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	UStaticMesh* tMesh = LoadObject<UStaticMesh>(NULL,
		TEXT("StaticMesh'/Game/M5VFXVOL2/Props/Meshes/BF_mesh_brazier.BF_mesh_brazier'"));
	if (tMesh)
	{
		MeshComp->SetStaticMesh(tMesh);
	}

	FirePSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FirePSC"));
	UParticleSystem* FireFX = LoadObject<UParticleSystem>(NULL,
		TEXT("ParticleSystem'/Game/M5VFXVOL2/Particles/Fire/Fire_03.Fire_03'"));
	if (FireFX)
	{
		FirePSC->SetTemplate(FireFX);
	}
}

void AFireBase::TurnOnLight()
{
	TurnOnFire();
}

void AFireBase::TurnOffLight()
{
	TurnOffFire();
}

void AFireBase::TurnOnFire()
{
	XanaduTools::LogScreen(FString::Printf(TEXT("点亮火焰: %s"), *GetDebugName(this)));
}

void AFireBase::TurnOffFire()
{

}
