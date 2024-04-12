// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIGAME_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 MaxAmmo = 20;


public:	
	
	void ReceiveDamage(float Damage);
	void UseAmmo(float Bullets);
	float GetHealthPercent();

	bool IsAlive();
	bool IsEquipped();

	void AddHealth(int32 HealthAmount);
	void AddToAmmo(int32 Bullets);
		
	FORCEINLINE int32 GetAmmo()const { return CurrentAmmo; }

};
