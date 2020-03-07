// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHealthBarWidget.h"
#include "_Xanadu/Characters/Enemy/Base/AICharacterBase.h"
#include "ProgressBar.h"
#include "_Xanadu/Characters/Base/Interfaces/IHealth.h"

bool UAIHealthBarWidget::Initialize()
{
	Super::Initialize();

	HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));

	return true;
}

void UAIHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetHealthBarPercent();
}

void UAIHealthBarWidget::SetHealthBarPercent()
{
	if (HealthBar && AICharacter)
	{
		float tHealth = IIHealth::Execute_GetHealth(AICharacter);
		float tHealthMax = IIHealth::Execute_GetHealthMax(AICharacter);
		float tPercent = tHealth / tHealthMax;
		HealthBar->SetPercent(tPercent);
	}
}

void UAIHealthBarWidget::SetAICharacter(class AAICharacterBase* inAI)
{
	if (inAI)
	{
		AICharacter = inAI;
	}
}
