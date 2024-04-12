// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DroneOverlay.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAME_API UDroneOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetHealthBarPercent(float Percent);

	void SetBulletsAmmo(int32 Bullets);

private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressHealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BulletsCountText;
};
