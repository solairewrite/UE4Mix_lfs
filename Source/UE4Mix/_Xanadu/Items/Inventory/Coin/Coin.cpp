// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Components/StaticMeshComponent.h"

ACoin::ACoin()
{
	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	SetRootComponent(CoinMesh);
	UStaticMesh* tCoinMesh = LoadObject<UStaticMesh>(NULL,
		TEXT("StaticMesh'/Game/AncientTreasures/Meshes/SM_Coin_Pile_01f.SM_Coin_Pile_01f'"));
	CoinMesh->SetStaticMesh(tCoinMesh);
}
