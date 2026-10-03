#include "Projectile.h"
#include "Asteroid.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	InitialLifeSpan = 3.f;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(30.f);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetGenerateOverlapEvents(true);
	SetRootComponent(Collision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 2500.f;
	Movement->MaxSpeed = 2500.f;
	Movement->ProjectileGravityScale = 0.f;
	Movement->bConstrainToPlane = true;
	Movement->SetPlaneConstraintNormal(FVector::UpVector);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlap);
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AAsteroid* Asteroid = Cast<AAsteroid>(OtherActor))
	{
		Asteroid->TakeHit(Damage);
		Destroy();
	}
}
