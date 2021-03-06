// Fill out your copyright notice in the Description page of Project Settings.


#include "XanaduUserWidget.h"
#include "ConstructorHelpers.h"
#include "TextBlock.h"
#include "Image.h"
#include "Button.h"
#include "_Xanadu/Base/XanaduTools.h"

// 基类没有无参构造函数,子类不能声明无参构造函数
UXanaduUserWidget::UXanaduUserWidget(const FObjectInitializer& ObjectInitializer) :UUserWidget(ObjectInitializer)
{
	AvatarImagePath = TEXT("Texture2D'/Game/CraftResourcesIcons/Textures/Tex_spare_parts_04_b.Tex_spare_parts_04_b'");
	PlayerName = TEXT("骑士");
}

bool UXanaduUserWidget::Initialize()
{
	Super::Initialize();

	InitAvatar();

	return true;
}

void UXanaduUserWidget::InitAvatar()
{
	AvatarButton = Cast<UButton>(GetWidgetFromName("AvatarButton"));
	if (AvatarButton)
	{
		AvatarButton->OnReleased.AddDynamic(this, &UXanaduUserWidget::OnClickAvatarButton);
	}

	AvatarImage = Cast<UImage>(GetWidgetFromName("AvatarImage"));
	if (AvatarImage)
	{
		static ConstructorHelpers::FObjectFinder<UTexture2D> tAvatarTex(*AvatarImagePath);
		if (tAvatarTex.Succeeded())
		{
			AvatarImage->SetBrushFromTexture(tAvatarTex.Object, true);
		}
	}

	PlayerNameText = Cast<UTextBlock>(GetWidgetFromName("PlayerNameText"));
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(*PlayerName));
	}
}

void UXanaduUserWidget::OnClickAvatarButton()
{
	XanaduTools::LogScreen(TEXT("点击了头像按钮"));
}
