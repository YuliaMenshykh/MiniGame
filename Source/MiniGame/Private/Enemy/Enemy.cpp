// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Weapon/EnemyWeapon.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SightConfig->SightRadius = CombatRadius;
		SightConfig->LoseSightRadius = CombatRadius + 1.f;
		SightConfig->PeripheralVisionAngleDegrees = 360.0f;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	}

	AIPerComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	if (AIPerComp)
	{
		AIPerComp->ConfigureSense(*SightConfig);
		AIPerComp->SetDominantSense(SightConfig->GetSenseImplementation());
	}

	

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (AIPerComp)
	{
		AIPerComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnSensed);
	}
	InitializeEnemy();
	Tags.Add(FName("Enemy"));

}

void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());

	ShowHealthBar(true);
	SpawnDefaultWeapon();

}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		EquipedWeapon = World->SpawnActor<AEnemyWeapon>(WeaponClass);
		EquipedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));
		EquipedWeapon->SetOwner(this);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::OnSensed(const TArray<AActor*>& DetectedPawns)
{
	if (DetectedPawns.Num() == 0)return;

	CombatTarget = DetectedPawns[0];
	CheckCombatTarget();
}

void AEnemy::CheckCombatTarget()
{
	if (IsInAttackRadius())
	{
		StartAttackTimer();
	}
	else if (!IsInAttackRadius())
	{
		ClearAttackTimer();
		LooseInterest();
	}

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes->IsAlive())
	{
		HandleDamage(DamageAmount);
		CombatTarget = EventInstigator->GetPawn();
	}
	else
	{
		Die_Implementation();
	}


	return DamageAmount;
}

void AEnemy::HandleDamage(float DamageAmount)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}

}

void AEnemy::ShowHealthBar(bool toShow)
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(toShow);
	}
}


bool AEnemy::IsInAttackRadius()
{
	return InRange(CombatTarget, CombatRadius);
}

void AEnemy::StartAttackTimer()
{
	const float RandTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, RandTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::Attack()
{
	if (CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}
	if (CombatTarget == nullptr)return;

	if (EquipedWeapon)
	{
		EquipedWeapon->FireButtonPressed(CombatTarget->GetActorLocation());
	}
}

void AEnemy::LooseInterest()
{
	CombatTarget = nullptr;
}

bool AEnemy::InRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;

	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::Die_Implementation()
{

	if (EquipedWeapon)
	{
		EquipedWeapon->Destroy();
	}

	ClearAttackTimer();
	ShowHealthBar(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(1);

}

