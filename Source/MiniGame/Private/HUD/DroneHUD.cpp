// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DroneHUD.h"
#include "HUD/DroneOverlay.h"

void ADroneHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && DroneOverlayClass)
		{
			DroneOverlay = CreateWidget<UDroneOverlay>(Controller, DroneOverlayClass);
			DroneOverlay->AddToViewport();
		}
	}
}
