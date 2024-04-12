// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/AmmoItem.h"
#include "Drone/Drone.h"

void AAmmoItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADrone* Drone = Cast<ADrone>(OtherActor);
	if (Drone)
	{
		Drone->AddAmmo(BulletsAmount);
		SpawnPickupEffect();
		SpawnPickupSound();
		Destroy();
	}
	
}
