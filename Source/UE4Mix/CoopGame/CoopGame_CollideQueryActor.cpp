// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame_CollideQueryActor.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACoopGame_CollideQueryActor::ACoopGame_CollideQueryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200.0f);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);
}

void ACoopGame_CollideQueryActor::Init(ACoopGame_STrackerBot* InBot)
{
	Bot = InBot;
	if (Bot)
	{
		FAttachmentTransformRules AttachRule = FAttachmentTransformRules::SnapToTargetIncludingScale;
		AttachToActor(Bot, AttachRule);
	}
}

// Called when the game starts or when spawned
void ACoopGame_CollideQueryActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoopGame_CollideQueryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), 200.0f, 16, FColor::Red, false, 0, 0, 1.0f);
}

void ACoopGame_CollideQueryActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Bot)
	{
		Bot->OnOverlap(OtherActor);
	}
}

