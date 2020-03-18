// Fill out your copyright notice in the Description page of Project Settings.


#include "Chest.h"
#include "_Xanadu/Base/XanaduTools.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AChest::AChest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChestBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestBase"));
	SetRootComponent(ChestBase);
	UStaticMesh* tChestBaseMesh = LoadObject<UStaticMesh>(NULL,
		TEXT("StaticMesh'/Game/AncientTreasures/Meshes/SM_Chest_02b.SM_Chest_02b'"));
	ChestBase->SetStaticMesh(tChestBaseMesh);

	ChestLid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestLid"));
	ChestLid->SetupAttachment(ChestBase);
	UStaticMesh* tChestLidMesh = LoadObject<UStaticMesh>(NULL,
		TEXT("StaticMesh'/Game/AncientTreasures/Meshes/SM_Chest_02a.SM_Chest_02a'"));
	ChestLid->SetStaticMesh(tChestLidMesh);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(ChestBase);
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AChest::IsCanOpenChest()
{
	if (!bNeedKey)
	{
		return true;
	}
	// 检测玩家是否有钥匙
	return true;
}

void AChest::OnChestOpened()
{
	DropInventory();
}

void AChest::DropInventory()
{
	XanaduTools::LogScreen(TEXT("宝箱发放道具"));
}

