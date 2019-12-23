// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame_NormalBox.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACoopGame_NormalBox::ACoopGame_NormalBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 模拟物理
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	SetRootComponent(MeshComp);

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ACoopGame_NormalBox::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACoopGame_NormalBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

