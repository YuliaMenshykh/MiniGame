// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "AmmoItem.generated.h"

/**
 * 
 */
UCLASS()
class MINIGAME_API AAmmoItem : public ABaseItem
{
	GENERATED_BODY()
	
protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

private:

	UPROPERTY(EditAnywhere)
	int32 BulletsAmount = 20;
};
