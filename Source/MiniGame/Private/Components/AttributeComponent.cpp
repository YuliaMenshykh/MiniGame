// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	CurrentAmmo = MaxAmmo;
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0, MaxHealth);
}

float UAttributeComponent::GetHealthPercent()
{
	return CurrentHealth / MaxHealth;
}

void UAttributeComponent::UseAmmo(float Bullets)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo - Bullets, 0, MaxAmmo);
}

bool UAttributeComponent::IsAlive()
{
	return CurrentHealth > 0.f;
}

bool UAttributeComponent::IsEquipped()
{
	return CurrentAmmo > 0;
}

void UAttributeComponent::AddHealth(int32 HealthAmount)
{
	CurrentHealth += HealthAmount;
}

void UAttributeComponent::AddToAmmo(int32 Bullets)
{
	CurrentAmmo += Bullets;
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

