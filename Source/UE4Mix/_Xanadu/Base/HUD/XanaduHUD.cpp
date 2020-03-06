// Fill out your copyright notice in the Description page of Project Settings.


#include "XanaduHUD.h"
#include "Engine/Canvas.h"

AXanaduHUD::AXanaduHUD()
{
	AvatarSize = 70.0f;
	PlayerName = TEXT("骑士");
}

void AXanaduHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawAvatar();
}

void AXanaduHUD::DrawAvatar()
{
	const float tMargin = 20.0f;

	if (AvatarTex)
	{
		const float tScale = AvatarSize / AvatarTex->GetSurfaceHeight();
		DrawTextureSimple(AvatarTex, Canvas->ClipX - AvatarSize - tMargin, tMargin, tScale);
	}

	float tNameStartX = Canvas->ClipX - AvatarSize - tMargin;
	float tNameStartY = tMargin + AvatarSize + 0.5 * tMargin;
	DrawText(PlayerName.ToString(), FColor::Yellow, tNameStartX, tNameStartY, nullptr);
}
