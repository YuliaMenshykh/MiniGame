// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Drone.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AWeapon;
class UAttributeComponent;
class APlayerWeapon;


UCLASS()
class MINIGAME_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	
	ADrone();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AWeapon> DefaultWeaponClass;

	void AddAmmo(int32 BulletsAmount);
	void UseBullets();
	void AddHealth(int32 HealthAmmount);

protected:

	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack();
	bool IsEquipped();
	bool IsAlive();
	void SpawnWeapon();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;
	void HandleDamage(float DamageAmount);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DroneMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

private:	

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DroneMesh;

	UPROPERTY()
	class UDroneOverlay* DroneOverlay;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	APlayerWeapon* EquipedWeapon;

};
