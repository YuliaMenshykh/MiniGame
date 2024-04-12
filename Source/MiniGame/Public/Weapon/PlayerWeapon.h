// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "PlayerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAME_API APlayerWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

	void FireButtonPressed();

private:

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	FVector HittedTarget;
	FHitResult HitResult;

};
