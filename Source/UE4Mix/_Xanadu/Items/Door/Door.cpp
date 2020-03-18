// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"
#include "_Xanadu/Base/XanaduTools.h"
#include "_Xanadu/Characters/Player/Base/PlayerCharacterBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("USceneComponent"));
	SetRootComponent(DefaultSceneRoot);

	CreateTriggers();
}

#if WITH_EDITOR
void ADoor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// 检测是否修改了,加载/卸载地图的bool值
	TArray<FName> tLoadMapPropertyNameArr;
	tLoadMapPropertyNameArr.Add(GET_MEMBER_NAME_CHECKED(ADoor, bLoadFrontLevel));
	tLoadMapPropertyNameArr.Add(GET_MEMBER_NAME_CHECKED(ADoor, bUnLoadFrontLevel));
	tLoadMapPropertyNameArr.Add(GET_MEMBER_NAME_CHECKED(ADoor, bLoadBackLevel));
	tLoadMapPropertyNameArr.Add(GET_MEMBER_NAME_CHECKED(ADoor, bUnLoadBackLevel));

	FName tName = PropertyChangedEvent.Property ?
		PropertyChangedEvent.Property->GetFName() :
		NAME_None;

	if (tLoadMapPropertyNameArr.Contains(tName))
	{
		OnChangeLoadMapProperties();
	}
}
#endif

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	AddLoadMapDelegates();
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

void ADoor::CreateTriggers()
{
	OpenDoorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("OpenDoorTrigger"));
	OpenDoorTrigger->SetupAttachment(RootComponent);
	OpenDoorTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OpenDoorTrigger->SetBoxExtent(FVector(600.0f, 250.0f, 160.0f));
	OpenDoorTrigger->SetRelativeLocation(FVector(0, 0, 160.0f));
	OpenDoorTrigger->ShapeColor = FColor(85, 223, 0);

	LoadFrontMapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LoadFrontMapTrigger"));
	InitTrigger(LoadFrontMapTrigger, FVector(-90.0f, 0, 160.0f), FColor(223, 178, 0));

	UnLoadFrontMapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("UnLoadFrontMapTrigger"));
	InitTrigger(UnLoadFrontMapTrigger, FVector(-670.0f, 0, 160.0f), FColor(223, 0, 50));

	LoadBackMapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("LoadBackMapTrigger"));
	InitTrigger(LoadBackMapTrigger, FVector(90.0f, 0, 160.0f), FColor(0, 212, 223));

	UnLoadBackMapTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("UnLoadBackMapTrigger"));
	InitTrigger(UnLoadBackMapTrigger, FVector(670.0f, 0, 160.0f), FColor(223, 0, 66));
}

void ADoor::InitTrigger(UBoxComponent* inTrigger, FVector inRelativeLoc, FColor inColor)
{
	if (!inTrigger)
	{
		return;
	}

	inTrigger->SetupAttachment(RootComponent);
	inTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	inTrigger->SetBoxExtent(FVector(80.0f, 250.0f, 160.0f));
	inTrigger->SetRelativeLocation(inRelativeLoc);
	inTrigger->ShapeColor = inColor;
	// 默认不显示线框,只有加载/卸载地图时才显示
	inTrigger->SetVisibility(false);
}

void ADoor::OnChangeLoadMapProperties()
{
	LoadFrontMapTrigger->SetVisibility(bLoadFrontLevel);
	UnLoadFrontMapTrigger->SetVisibility(bUnLoadFrontLevel);

	LoadBackMapTrigger->SetVisibility(bLoadBackLevel);
	UnLoadBackMapTrigger->SetVisibility(bUnLoadBackLevel);
}

void ADoor::AddLoadMapDelegates()
{
	OpenDoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnEnterOpenDoorTrigger);
	OpenDoorTrigger->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnLeaveOpenDoorTrigger);

	LoadFrontMapTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnEnterLoadFrontMapTrigger);
	UnLoadFrontMapTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnEnterUnLoadFrontMapTrigger);

}

void ADoor::OnEnterOpenDoorTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacterBase>(OtherActor))
	{
		OpenDoor();
	}
}

void ADoor::OnLeaveOpenDoorTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APlayerCharacterBase>(OtherActor))
	{
		CloseDoor();
	}
}

void ADoor::OnEnterLoadFrontMapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bLoadFrontLevel && !FrontMapName.IsNone())
	{
		FString tInfo = FString::Printf(TEXT("加载地图: %s"), *FrontMapName.ToString());
		XanaduTools::LogScreen(tInfo);

		// 动态加载地图
		// Window -> Levels调出关卡面板
		// 点击Summon Level Details按钮,调出Level Details面板,设置关卡初始是否加载
		FLatentActionInfo tLatentInfo;
		UGameplayStatics::LoadStreamLevel(this, FrontMapName, true, true, tLatentInfo);
	}
}

void ADoor::OnEnterUnLoadFrontMapTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bUnLoadFrontLevel && !FrontMapName.IsNone())
	{
		FString tInfo = FString::Printf(TEXT("卸载地图: %s"), *FrontMapName.ToString());
		XanaduTools::LogScreen(tInfo);

		// 动态卸载地图
		FLatentActionInfo tLatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, FrontMapName, tLatentInfo, false);
	}
}

