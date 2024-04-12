// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone/Drone.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "HUD/DroneOverlay.h"
#include "HUD/DroneHUD.h"
#include "Components/AttributeComponent.h"
#include "Weapon/PlayerWeapon.h"


ADrone::ADrone()
{
 	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(GetRootComponent());

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 100.0f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DroneMappingContext, 0);
		}

		ADroneHUD* DroneHUD = Cast<ADroneHUD>(PlayerController->GetHUD());
		if (DroneHUD)
		{
			DroneOverlay = DroneHUD->GetDroneOverlay();
			if (DroneOverlay && Attributes)
			{
				DroneOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				DroneOverlay->SetBulletsAmmo(Attributes->GetAmmo());
			}
		}
	}

	SpawnWeapon();
	Tags.Add(FName("Player"));
}

void ADrone::SpawnWeapon()
{
	UWorld* World = GetWorld();
	if (World)
	{
		EquipedWeapon = World->SpawnActor<APlayerWeapon>(DefaultWeaponClass);
		EquipedWeapon->AttachToComponent(DroneMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));
		EquipedWeapon->SetOwner(this);
	}
}

float ADrone::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser && IsAlive())
	{
		HandleDamage(DamageAmount);
	}
	else
	{
		Tags.Add(FName("Dead"));
	}
	return DamageAmount;
}

void ADrone::HandleDamage(float DamageAmount)
{
	if (Attributes && DroneOverlay)
	{
		Attributes->ReceiveDamage(DamageAmount);
		DroneOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

void ADrone::Move(const FInputActionValue& Value)
{
	if (IsAlive())
	{
		const FVector3d DirectionValue = Value.Get<FVector3d>();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, DirectionValue.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, DirectionValue.X);

		const FVector VerticalDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
		AddMovementInput(VerticalDirection, DirectionValue.Z);
	}

}

void ADrone::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ADrone::Attack()
{
	if (EquipedWeapon && IsAlive() && IsEquipped())
	{
		EquipedWeapon->FireButtonPressed();
		UseBullets();
	}
}

bool ADrone::IsEquipped()
{
	return Attributes->IsEquipped();
}

bool ADrone::IsAlive()
{
	return Attributes->IsAlive();
}

void ADrone::AddAmmo(int32 BulletsAmount)
{
	if (Attributes && DroneOverlay)
	{
		Attributes->AddToAmmo(BulletsAmount);
		DroneOverlay->SetBulletsAmmo(Attributes->GetAmmo());

	}
}

void ADrone::UseBullets()
{
	if (Attributes && DroneOverlay)
	{
		Attributes->UseAmmo(1);
		DroneOverlay->SetBulletsAmmo(Attributes->GetAmmo());
	}
}

void ADrone::AddHealth(int32 HealthAmmount)
{
	if (Attributes && DroneOverlay)
	{
		Attributes->AddHealth(HealthAmmount);
		DroneOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}


void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADrone::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADrone::Look);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ADrone::Attack);
	}
}

