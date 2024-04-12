// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class AWeapon;
class UAttributeComponent;
class UHealthBarComponent;
class AEnemyWeapon;

UCLASS()
class MINIGAME_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:

	AEnemy();
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;
	
	UFUNCTION(BlueprintNativeEvent)
	void Die();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Combat)
	AActor* CombatTarget;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;


private:	

	void InitializeEnemy();
	void SpawnDefaultWeapon();
	bool IsInAttackRadius();
	void CheckCombatTarget();
	void StartAttackTimer();
	void ClearAttackTimer();
	void Attack();
	void LooseInterest();
	bool InRange(AActor* Target, double Radius);
	void ShowHealthBar(bool toShow);
	void HandleDamage(float DamageAmount);

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float CombatRadius = 1000;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.1f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 0.7f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AEnemyWeapon* EquipedWeapon;

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(VisibleAnywhere, Category = Enemy)
	class UAIPerceptionComponent* AIPerComp;

	UPROPERTY(VisibleAnywhere, Category = Enemy)
	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnSensed(const TArray<AActor*>& DetectedPawns);

};
