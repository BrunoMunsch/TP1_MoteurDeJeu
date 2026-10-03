#include "SpaceShip.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"

ASpaceShip::ASpaceShip()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Mesh->SetGenerateOverlapEvents(true);
}

void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
	FString::Printf(TEXT("Setup: Ctx=%d Move=%d Fire=%d"),
		MappingContext != nullptr, MoveAction != nullptr, FireAction != nullptr));
}

void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (MappingContext)
			{
				Subsystem->AddMappingContext(MappingContext, 0);
				
			}
		}
	}

	if (auto* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpaceShip::Move);
			EIC->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASpaceShip::StopMove);
			EIC->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ASpaceShip::StopMove);
		}
		if (FireAction)
		{
			EIC->BindAction(FireAction, ETriggerEvent::Triggered, this, &ASpaceShip::Fire);
		}
	}
}

void ASpaceShip::Move(const FInputActionValue& Value)
{	
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, TEXT("MOVE OK"));
	MoveInput = Value.Get<FVector2D>();
}

void ASpaceShip::StopMove(const FInputActionValue& Value)
{
	MoveInput = FVector2D::ZeroVector;
}

void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// MoveInput.X = droite/gauche (axe Y du monde), MoveInput.Y = haut/bas (axe X du monde)
	FVector NewLoc = GetActorLocation();
	NewLoc.X += MoveInput.Y * MoveSpeed * DeltaTime;
	NewLoc.Y += MoveInput.X * MoveSpeed * DeltaTime;
	NewLoc.X = FMath::Clamp(NewLoc.X, -BoundsX, BoundsX);
	NewLoc.Y = FMath::Clamp(NewLoc.Y, -BoundsY, BoundsY);
	SetActorLocation(NewLoc);
}

void ASpaceShip::Fire(const FInputActionValue& Value)
{
	if (!ProjectileClass) return;

	const float Now = GetWorld()->GetTimeSeconds();
	if (Now - LastFireTime < FireCooldown) return;
	LastFireTime = Now;

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FVector SpawnLoc = GetActorLocation() + GetActorForwardVector() * MuzzleOffset;
	GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLoc, GetActorRotation(), Params);

	OnFireFX();
}
