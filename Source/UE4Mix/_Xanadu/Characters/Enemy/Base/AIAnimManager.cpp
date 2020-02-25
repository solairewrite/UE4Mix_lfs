// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAnimManager.h"
#include "AIControllerBase.h"
#include "AICharacterBase.h"

// Sets default values
AAIAnimManager::AAIAnimManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIAnimManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAIAnimManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIAnimManager::Init(AAIControllerBase* inController)
{
	OwnerController = inController;
}

float AAIAnimManager::PlayAnim(FName inAnimName)
{
	AAICharacterBase* character = GetCharacter();
	if (character)
	{
		float animLength = character->PlayAnim(inAnimName);
		return animLength;
	}
	return 0;
}

AAICharacterBase* AAIAnimManager::GetCharacter()
{
	if (OwnerController)
	{
		return Cast<AAICharacterBase>(OwnerController->GetCharacter());
	}
	return nullptr;
}

