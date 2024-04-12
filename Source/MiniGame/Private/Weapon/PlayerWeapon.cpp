// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PlayerWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceUnderCrosshairs(HitResult);
}

void APlayerWeapon::FireButtonPressed()
{
	Fire(HittedTarget);
}

void APlayerWeapon::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	FVector2D CrosshairLocation(ViewportSize.X / 2, ViewportSize.Y / 2);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);
	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		FVector End = Start + CrosshairWorldDirection * TraceLength;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
			HittedTarget = End;
		}
		else
		{
			HittedTarget = TraceHitResult.ImpactPoint;
			DrawDebugSphere(
				GetWorld(),
				TraceHitResult.ImpactPoint,
				12.f,
				12.f,
				FColor::Red
			);
		}
	}
}
