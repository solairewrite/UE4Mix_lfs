// Fill out your copyright notice in the Description page of Project Settings.


#include "XanaduHUD.h"
#include "Engine/Canvas.h"
#include "ConstructorHelpers.h"
#include "_Xanadu/Base/HUD/XanaduUserWidget.h"

AXanaduHUD::AXanaduHUD()
{
	if (!GEngine || !GEngine->GameViewport)
	{
		return;
	}

	// 在编辑器中选中资源Ctrl+C可复制路径,前面有类型名,后面要加"_C"
	static ConstructorHelpers::FClassFinder<UUserWidget> tWidget(
		TEXT("WidgetBlueprint'/Game/_MyProjects/_Xanadu/Characters/Player/Knight/HUD/WBP_Knight.WBP_Knight_C'"));
	if (tWidget.Succeeded())
	{
		WidgetClass = tWidget.Class;

		UserWidget = CreateWidget<UXanaduUserWidget>(GetWorld()->GetGameInstance(), WidgetClass);
		if (UserWidget)
		{
			UserWidget->AddToViewport();
		}
	}
}

void AXanaduHUD::DrawHUD()
{
	Super::DrawHUD();
}
