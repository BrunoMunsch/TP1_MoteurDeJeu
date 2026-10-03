#include "Asteroid.h"
#include "SpaceShip.h"
#include "SpaceGameMode.h"
#include "Components/StaticMeshComponent.h"

AAsteroid::AAsteroid()
{
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 25.f; // sécurité : se détruit s'il ne croise jamais le joueur

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAsteroid::OnOverlap);
}

void AAsteroid::BeginPlay()
{
	Super::BeginPlay();

	Health = FMath::RandRange(MinHealth, MaxHealth);
	SetActorScale3D(FVector(FMath::FRandRange(MinScale, MaxScale)));
	SpinRate = FRotator(
		FMath::FRandRange(-MaxSpinSpeed, MaxSpinSpeed),
		FMath::FRandRange(-MaxSpinSpeed, MaxSpinSpeed),
		FMath::FRandRange(-MaxSpinSpeed, MaxSpinSpeed));
}

void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldOffset(Velocity * DeltaTime);
	AddActorLocalRotation(SpinRate * DeltaTime);
}

void AAsteroid::TakeHit(int32 Damage)
{
	Health -= Damage;
	if (Health <= 0)
	{
		Explode(true);
	}
	else
	{
		OnHitFX(GetActorLocation());
	}
}

void AAsteroid::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ASpaceShip>(OtherActor))
	{
		if (ASpaceGameMode* GM = GetWorld()->GetAuthGameMode<ASpaceGameMode>())
		{
			GM->LoseLife();
		}
		Explode(false);
	}
}

void AAsteroid::Explode(bool bGivePoints)
{
	if (bGivePoints)
	{
		if (ASpaceGameMode* GM = GetWorld()->GetAuthGameMode<ASpaceGameMode>())
		{
			GM->AddScore(ScoreValue);
		}
	}
	OnDestroyedFX(GetActorLocation());
	Destroy();
}
