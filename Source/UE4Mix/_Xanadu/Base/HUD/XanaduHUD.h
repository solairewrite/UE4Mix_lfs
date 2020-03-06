// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "XanaduHUD.generated.h"

/**
 *
 */
UCLASS()
class UE4MIX_API AXanaduHUD : public AHUD
{
	GENERATED_BODY()

public:
	AXanaduHUD();

	// 重写基类绘制函数
	virtual void DrawHUD() override;

protected:
	// 主要的UI
	class UXanaduUserWidget* UserWidget;
	// 蓝图Widget类
	TSubclassOf<class UXanaduUserWidget> WidgetClass;
};
