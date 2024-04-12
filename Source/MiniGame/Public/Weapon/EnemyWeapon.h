// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "EnemyWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAME_API AEnemyWeapon : public AWeapon
{
	GENERATED_BODY()
	
public :

	void FireButtonPressed(const FVector& HitTarget);

};
