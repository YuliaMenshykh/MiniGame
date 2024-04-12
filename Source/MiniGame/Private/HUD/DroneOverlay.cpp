// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DroneOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UDroneOverlay::SetHealthBarPercent(float Percent)
{
	if (ProgressHealthBar)
	{
		ProgressHealthBar->SetPercent(Percent);
	}
}

void UDroneOverlay::SetBulletsAmmo(int32 Bullets)
{
	if (BulletsCountText)
	{
		BulletsCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Bullets)));
	}
}
