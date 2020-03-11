// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"

// Sets default values
ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("USceneComponent"));
	SetRootComponent(DefaultSceneRoot);

	OpenDoorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("OpenDoorTrigger"));
	OpenDoorTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OpenDoorTrigger->SetupAttachment(RootComponent);

	LoadFrontMapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LoadFrontMapTrigger"));
	LoadFrontMapTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LoadFrontMapTrigger->SetupAttachment(RootComponent);

	UnLoadFrontMapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("UnLoadFrontMapTrigger"));
	UnLoadFrontMapTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UnLoadFrontMapTrigger->SetupAttachment(RootComponent);

	LoadBackMapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LoadBackMapTrigger"));
	LoadBackMapTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LoadBackMapTrigger->SetupAttachment(RootComponent);

	UnLoadBackMapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("UnLoadBackMapTrigger"));
	UnLoadBackMapTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UnLoadBackMapTrigger->SetupAttachment(RootComponent);
}

void ADoor::OnDoorOpened()
{
	// 广播事件只能C++调用
	OnDoorOpenedDelegate.Broadcast();
}

void ADoor::OnDoorClosed()
{
	OnDoorClosedDelegate.Broadcast();
}

