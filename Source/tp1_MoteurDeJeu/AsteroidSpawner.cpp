#include "AsteroidSpawner.h"
#include "Asteroid.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"

AAsteroidSpawner::AAsteroidSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
}

void AAsteroidSpawner::BeginPlay()
{
	Super::BeginPlay();
	ScheduleNext();
}

void AAsteroidSpawner::ScheduleNext()
{
	const float Delay = FMath::FRandRange(MinDelay, MaxDelay);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AAsteroidSpawner::SpawnAsteroid, Delay, false);
}

void AAsteroidSpawner::SpawnAsteroid()
{
	if (AsteroidClass)
	{
		const FVector Center = GetActorLocation();
		const float X = HalfExtentX + Margin;
		const float Y = HalfExtentY + Margin;

		FVector Offset = FVector::ZeroVector;
		switch (FMath::RandRange(0, 3))
		{
		case 0: Offset = FVector(X, FMath::FRandRange(-HalfExtentY, HalfExtentY), 0.f); break;  // haut
		case 1: Offset = FVector(-X, FMath::FRandRange(-HalfExtentY, HalfExtentY), 0.f); break; // bas
		case 2: Offset = FVector(FMath::FRandRange(-HalfExtentX, HalfExtentX), Y, 0.f); break;  // droite
		default: Offset = FVector(FMath::FRandRange(-HalfExtentX, HalfExtentX), -Y, 0.f); break; // gauche
		}
		const FVector SpawnLoc = Center + Offset;

		// Cible : le joueur ou un point aléatoire dans la zone
		FVector Target = Center + FVector(
			FMath::FRandRange(-HalfExtentX * 0.5f, HalfExtentX * 0.5f),
			FMath::FRandRange(-HalfExtentY * 0.5f, HalfExtentY * 0.5f), 0.f);

		if (APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0))
		{
			if (FMath::FRand() < AimAtPlayerChance)
			{
				Target = Player->GetActorLocation();
			}
		}

		FVector Dir = Target - SpawnLoc;
		Dir.Z = 0.f;
		Dir.Normalize();

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (AAsteroid* Asteroid = GetWorld()->SpawnActor<AAsteroid>(AsteroidClass, SpawnLoc, FRotator::ZeroRotator, Params))
		{
			Asteroid->Velocity = Dir * FMath::FRandRange(MinSpeed, MaxSpeed);
		}
	}
	ScheduleNext();
}
