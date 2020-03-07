// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AIHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE4MIX_API UAIHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize();
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 不能直接获取WidgetComponent,需要显式设置
	class AAICharacterBase* AICharacter;

	class UProgressBar* HealthBar;

	void SetHealthBarPercent();

public:
	void SetAICharacter(class AAICharacterBase* inAI);
};
