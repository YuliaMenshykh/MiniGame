// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DroneHUD.generated.h"

class UDroneOverlay;
/**
 * 
 */
UCLASS()
class MINIGAME_API ADroneHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay()override;

	UPROPERTY(EditDefaultsOnly, Category = Drone)
	TSubclassOf<UDroneOverlay>DroneOverlayClass;

private:
	UPROPERTY()
	UDroneOverlay* DroneOverlay;

public:

	FORCEINLINE UDroneOverlay* GetDroneOverlay()const { return DroneOverlay; }

};
