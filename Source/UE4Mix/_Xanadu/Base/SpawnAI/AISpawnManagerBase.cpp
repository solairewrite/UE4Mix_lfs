// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawnManagerBase.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BillboardComponent.h"
#include "Engine/Texture2D.h"
#include "Components/SphereComponent.h"
#include "_Xanadu/Characters/Enemy/Base/AICharacterBase.h"

// Sets default values
AAISpawnManagerBase::AAISpawnManagerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxSpawnAICount = 1;
	bCanRepeatSpawnAI = true;
	SpawnAICD = 10.0f;

	SpawnAIRadius = 1500.0f;
	KillAIRadius = 15000.0f;
	AIWanderRadius = 500.0f;

	Icon = CreateDefaultSubobject<UBillboardComponent>(TEXT("IconBillboardComp"));
	Icon->SetupAttachment(RootComponent);
	UTexture2D* tIconTex = LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Spawn_System/Textures/T_Icon_AI_Spawn.T_Icon_AI_Spawn'"));
	if (tIconTex)
	{
		Icon->SetSprite(tIconTex);
		Icon->ScreenSize = 0.0001f;
	}

	SpawnSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ShowSpawnRadiusSphereComp"));
	SpawnSphere->SetupAttachment(RootComponent);
	SpawnSphere->SetSphereRadius(SpawnAIRadius);
	SpawnSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnSphere->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AAISpawnManagerBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TH_CheckSpawnAI, this, &AAISpawnManagerBase::CheckSpawnAI, 0.2f, true);
}

// Called every frame
void AAISpawnManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAISpawnManagerBase::CheckSpawnAI()
{
	// 销毁AI
	float tDis = GetDistanceToPlayer();
	if (tDis > KillAIRadius)
	{
		DestroyAllAI();
	}

	// 清除对死亡AI的引用
	ClearDeadAI();

	if (!IsCanSpawnAI())
	{
		return;
	}

	SpawnAI();
}

bool AAISpawnManagerBase::IsCanSpawnAI()
{
	if (CurrentAICount >= MaxSpawnAICount)
	{
		return false;
	}

	if (SpawnAICD > 0 && LastSpawnAITime > 0 && GetWorld()->TimeSeconds - LastSpawnAITime <= SpawnAICD)
	{
		return false;
	}

	// 检测玩家是否进入可以生成AI的区域
	float tDis = GetDistanceToPlayer();
	if (tDis < 0 || tDis > SpawnAIRadius)
	{
		return false;
	}

	return true;
}

void AAISpawnManagerBase::SpawnAI()
{
	LastSpawnAITime = GetWorld()->TimeSeconds;
}

AAICharacterBase* AAISpawnManagerBase::SpawnAAI(TSubclassOf<AAICharacterBase> inAIClass, FVector inSpawnLoc, FRotator inSpawnRot /*= FRotator::ZeroRotator*/)
{
	FActorSpawnParameters tSpawnPara;
	tSpawnPara.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AAICharacterBase* tAI = GetWorld()->SpawnActor<AAICharacterBase>(inAIClass, inSpawnLoc, inSpawnRot, tSpawnPara);

	if (tAI)
	{
		CurrentAICount++;
		AllSpawnedAICount++;
	}

	return tAI;
}

void AAISpawnManagerBase::DestroyAllAI()
{
	CurrentAICount = 0;
}

float AAISpawnManagerBase::GetDistanceToPlayer()
{
	ACharacter* tPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!tPlayer)
	{
		return -1;
	}
	float tDis = GetDistanceTo(tPlayer);
	return tDis;
}

void AAISpawnManagerBase::ClearDeadAI()
{

}

