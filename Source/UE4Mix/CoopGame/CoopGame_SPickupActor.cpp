// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGame_SPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "TimerManager.h"
#include "CoopGame_SPowerupActor.h"

// Sets default values
ACoopGame_SPickupActor::ACoopGame_SPickupActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACoopGame_SPickupActor::BeginPlay()
{
	Super::BeginPlay();

	Respawn();
}

void ACoopGame_SPickupActor::Respawn()
{
	if (PowerupClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("在%s中,PowerupClass == nullptr,请在蓝图中设置"), *GetName());
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PowerupInstance = GetWorld()->SpawnActor<ACoopGame_SPowerupActor>(PowerupClass, GetTransform(), SpawnParams);
}

// Called every frame
void ACoopGame_SPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoopGame_SPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (PowerupInstance)
	{
		PowerupInstance->ActivatePowerup();
		PowerupInstance = nullptr;
	}

	// 重生
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ACoopGame_SPickupActor::Respawn, CooldownDuration);
}

